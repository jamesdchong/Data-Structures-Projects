subdir('DictionaryTrie')
subdir('Util')

autocomplete_exe = executable('autocomplete.cpp.executable',
    sources: ['autocomplete.cpp'],
    dependencies : [dictionary_trie_dep, util_dep],
    install : true)

benchtrie_exe = executable('benchtrie.cpp.executable', 
    sources: ['benchtrie.cpp'],
    dependencies : [dictionary_trie_dep, util_dep],
    install : true)
