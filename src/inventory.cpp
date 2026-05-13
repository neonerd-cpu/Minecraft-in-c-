#include "inventory.h"
#include <vector>

Inventory::Inventory() : selectedIndex(0) {
    hotbar = {BlockType::Grass, BlockType::Dirt, BlockType::Stone, BlockType::Wood, BlockType::Leaf};
    for (auto type : hotbar) {
        items[type] = 0;
    }
    selectedType = hotbar[selectedIndex];
}

void Inventory::addBlock(BlockType type, int count) {
    if (type != BlockType::Air) {
        items[type] += count;
    }
}

void Inventory::removeBlock(BlockType type, int count) {
    if (items.count(type)) {
        items[type] = std::max(0, items[type] - count);
    }
}

int Inventory::getCount(BlockType type) const {
    if (items.count(type)) return items.at(type);
    return 0;
}

void Inventory::selectNext() {
    selectedIndex = (selectedIndex + 1) % hotbar.size();
    selectedType = hotbar[selectedIndex];
}

void Inventory::selectPrev() {
    selectedIndex = (selectedIndex - 1 + hotbar.size()) % hotbar.size();
    selectedType = hotbar[selectedIndex];
}
