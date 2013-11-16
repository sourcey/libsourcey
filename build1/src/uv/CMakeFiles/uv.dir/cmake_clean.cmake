FILE(REMOVE_RECURSE
  "libscy_uv.pdb"
  "libscy_uv.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/uv.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
