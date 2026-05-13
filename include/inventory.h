#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>
#include <vector>
#include "block.h"

class Inventory {
public:
    Inventory();
    
    void addBlock(BlockType type, int count = 1);
    void removeBlock(BlockType type, int count = 1);
    int getCount(BlockType type) const;
    
    BlockType getSelectedType() const { return selectedType; }
    void selectNext();
    void selectPrev();

private:
    std::map<BlockType, int> items;
    BlockType selectedType;
    std::vector<BlockType> hotbar;
    int selectedIndex;
};

#endif
