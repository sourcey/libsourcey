#!/usr/bin/env node

import { existsSync, readdirSync, readFileSync } from "node:fs";
import { dirname, join, relative, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const root = resolve(here, "..", "..");
const args = process.argv.slice(2);
const distDir = resolve(process.cwd(), args.find((arg) => !arg.startsWith("--")) ?? "dist");
const verifyPublicLinks = process.argv.includes("--verify-public-links");
const failures = [];

function fail(message) {
  failures.push(message);
}

function readText(path) {
  return readFileSync(path, "utf8");
}

function collectFiles(dir, predicate, out = []) {
  if (!existsSync(dir)) return out;
  for (const entry of readdirSync(dir, { withFileTypes: true })) {
    const full = join(dir, entry.name);
    if (entry.isDirectory()) {
      collectFiles(full, predicate, out);
    } else if (predicate(full, entry.name)) {
      out.push(full);
    }
  }
  return out;
}

const apiDir = join(distDir, "api-reference");
const searchDir = join(distDir, "_search");
// Search is published per language and version. Each context's entry names the directory of its
// result fragments, eight results each, written as [title, address, category, tab, method, path,
// qualified name, owner], the category and tab by their place in the entry's lists. An address
// that starts with "#" is on the page of the result before it in its fragment.
const searchContexts = collectFiles(searchDir, (_full, name) => /^context-.+\.json$/.test(name));
const fragmentNumber = (file) => Number(file.match(/f(\d+)\.json$/)[1]);

function searchRows(contextFile) {
  const entry = JSON.parse(readText(contextFile));
  const fragments = collectFiles(join(searchDir, entry.files), (_full, name) => /^f\d+\.json$/.test(name))
    .sort((a, b) => fragmentNumber(a) - fragmentNumber(b));
  return fragments.flatMap((file) => {
    let page = "";
    return JSON.parse(readText(file)).map(([title, url, category, tab, method, path, qualifiedName, owner]) => {
      const address = url.startsWith("#") ? `${page}${url}` : url;
      page = address.split("#")[0];
      return { title, url: address, category: entry.categories[category], tab: entry.tabs[tab], method, path, qualifiedName, owner };
    });
  });
}
const apiPages = collectFiles(apiDir, (_full, name) => name.endsWith(".html"));

if (apiPages.length < 400) {
  fail(`Expected at least 400 API pages, found ${apiPages.length}`);
}

for (const file of apiPages) {
  const name = relative(apiDir, file);
  if (/[() ]/.test(name)) {
    fail(`Unsafe API filename: ${name}`);
  }
}

if (!searchContexts.length) {
  fail("Missing dist/_search search contexts");
} else {
  const entries = searchContexts.flatMap(searchRows);
  if (entries.length < 7500) {
    fail(`Expected at least 7500 search entries, found ${entries.length}`);
  }

  const serialized = JSON.stringify(entries).toLowerCase();
  if (serialized.includes("pluga")) {
    fail("Search index still contains pluga");
  }

  const requireSymbol = entries.find((entry) =>
    entry.title === "requireSymbol" ||
    entry.qualifiedName === "icy::graft::Library::requireSymbol"
  );
  if (!requireSymbol) {
    fail("Search index is missing graft Library::requireSymbol");
  }
  if (requireSymbol && requireSymbol.category === "Pages") {
    fail("requireSymbol search hit resolves as a page, not a member");
  }

  const exactMember = entries.find((entry) => entry.qualifiedName && entry.url?.includes("#"));
  if (!exactMember) {
    fail("Search index has no qualified member metadata");
  }
}

const asyncQueue = join(apiDir, "icy-AsyncQueue.html");
if (!existsSync(asyncQueue)) {
  fail("Missing AsyncQueue API page");
} else {
  const html = readText(asyncQueue);
  if (!/template|class T|typename T/.test(html)) {
    fail("AsyncQueue page is missing class template parameters");
  }
  if (!html.includes("AsyncQueue") || !html.includes("limit") || !html.includes("2048")) {
    fail("AsyncQueue constructor is missing default argument value");
  }
  if (!html.includes("Defined in src/base/include/icy/queue.h:")) {
    fail("AsyncQueue page is missing source location");
  }
}

const appPage = join(apiDir, "icy-Application.html");
if (!existsSync(appPage)) {
  fail("Missing Application API page");
} else {
  const html = readText(appPage);
  if (!html.includes("Example") || !html.includes("waitForShutdown")) {
    fail("Application page is missing the Doxygen-sourced example block");
  }
  if (html.includes("[icy/application.h]") || html.includes("app.[")) {
    fail("Markdown links leaked into the Application code example");
  }
}

const graftPages = apiPages.filter((file) => /graft/i.test(file));
if (!graftPages.length) {
  fail("Missing graft API pages");
} else {
  const graftHtml = graftPages.map(readText).join("\n");
  if (!graftHtml.includes("Defined in src/graft/include/icy/graft/graft.h:")) {
    fail("graft page is missing plain source locations");
  }
  if (graftHtml.includes("github.com/nilstate/icey/blob/main/src/graft/")) {
    fail("graft page contains public source links for the graft submodule");
  }
}

const combinedApi = apiPages.map(readText).join("\n");
for (const marker of ["#### Parameters", "#### Template Parameters", "#### Exceptions"]) {
  if (combinedApi.includes(`<td>${marker}`) || combinedApi.includes(`| ${marker}`)) {
    fail(`Markdown heading leaked into an API table cell: ${marker}`);
  }
}

const linkedSourceUrls = [...combinedApi.matchAll(/https:\/\/github\.com\/nilstate\/icey\/blob\/main\/([^"#]+)(?:#L(\d+))?/g)];
for (const [, encodedPath, line] of linkedSourceUrls) {
  const sourcePath = decodeURIComponent(encodedPath);
  if (sourcePath.startsWith("src/graft/")) {
    fail(`graft submodule path was publicly linked: ${sourcePath}`);
    continue;
  }
  if (!existsSync(join(root, sourcePath))) {
    fail(`Linked source path does not exist locally: ${sourcePath}`);
  }
  if (line && !/^\d+$/.test(line)) {
    fail(`Linked source line is not numeric for ${sourcePath}: ${line}`);
  }
}

if (verifyPublicLinks) {
  const unique = [...new Set(linkedSourceUrls.slice(0, 100).map((match) => match[0]))];
  for (const url of unique) {
    const response = await fetch(url, { method: "HEAD" });
    if (!response.ok) {
      fail(`Public source link failed ${response.status}: ${url}`);
    }
  }
}

if (failures.length) {
  console.error("Sourcey C++ quality check failed:");
  for (const failure of failures) {
    console.error(`  - ${failure}`);
  }
  process.exit(1);
}

console.log("Sourcey C++ quality check passed.");
