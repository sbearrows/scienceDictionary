Final project for CSPB Data Structures class.

A searchable dictionary ADT using a hash table as the underlying structure. This type of ADT can be useful when collaborating with other laboratries so prevent non-descriptive or duplicated variable names during the scientific process.

The dictionary prevents users from inserting variables with the same name or the same definition. It also allows users to over-write variables to re-define them. It has a grep like functionality to search the definition portion and of any entry. If I were to re-implement this ADT I would use an AVL tree as the underlying structure rather than a hash table specifically because pattern recognition would be much faster for an AVL tree. Finally, it can output the file as a new csv, sorted alphabetically.

