#ifndef DICTIONARY_H_
#define DICTIONARY_H_


enum dicionary_entryType{   ///Opcode: Directly translatable to a single instruction
                            OPCODE,

                            ///An explicit address, defined as text
                            ADDRESS, 
                            
                            ///
                            LABEL};


struct dictionaryEntry{
    enum dictionaryType entryType;
    char entryText*;
    
};

#endif