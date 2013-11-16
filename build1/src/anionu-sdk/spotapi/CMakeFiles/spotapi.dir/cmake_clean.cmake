FILE(REMOVE_RECURSE
  "libscy_spotapi.pdb"
  "libscy_spotapi.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/spotapi.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
