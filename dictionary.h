#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <stdint.h>

enum dicionary_entryType{   ///An explicit unsigned 16-bit or smaller address, spelled out in ASCII in base 10.
                            ADDRESS, 
                            
                            ///A label referencing an instruction
                            LABEL,
                            
                            ///Not known or otherwise invalid.
                            UNKNOWN};


struct dictionaryEntry{
    enum dicionary_entryType entryType;
    uint8_t isValid;
    char *entryText;
    char *entryValue;
};

/**
 * @brief Calculates the unsigned 16-bit value for the entry given. If the entry has no value, 0xFFFF is returned.
 * 
 * @param entry the dictionary entry to calculate the value of
 * @return uint16_t the value stored in the entry
 */
uint16_t dictionary_getValue(struct dictionaryEntry entry);


/**
 * @brief Makes an entry into the dictionary for the specified label name. Note that the label does not have an address
 * that it is pointing to when first created--that will have to be specified later.
 * 
 * @param label 
 */
void dictionary_makeLabelEntry(char* label);

/**
 * @brief Fills in the address at the specified label so that any code referencing it understands what value it holds.
 * 
 * @param label the label to update the address of
 * @param address the address to set the label to point to
 * @return uint8_t 0 if successful, 1 if label not found. (should never occur)
 */
uint8_t dictionary_specifyLabelAddress(char* label, uint16_t romAddress);



#endif