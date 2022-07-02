#pragma once

#include <unordered_set>
#include <queue>

#include "Core/Core.h"

class Config;
class ConfigSection;

struct ConfigIterator 
{
private:
	std::queue<std::string> m_Names;
    Config* m_Config;

public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Config;
    using pointer           = value_type*;
    using reference         = value_type&;

    ConfigIterator();
    ConfigIterator(Config* cfg);

    reference operator*() const;
    pointer operator->();

    ConfigIterator& operator++();
    ConfigIterator operator++(int);

    friend bool operator== (const ConfigIterator& a, const ConfigIterator& b);
    friend bool operator!= (const ConfigIterator& a, const ConfigIterator& b);

};
