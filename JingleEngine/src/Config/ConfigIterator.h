#pragma once

#include <queue>

#include "Core/Core.h"

class Config;
class ConfigSection;

typedef std::unordered_map<std::string, Ref<Config>> ConfigMap;

struct ConfigIterator 
{
private:
	std::queue<ConfigSection*> m_Sections;
	ConfigMap::iterator m_IT;

public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Config;
    using pointer           = value_type*;
    using reference         = value_type&;

    ConfigIterator();
    ConfigIterator(Config* cfg);
    ConfigIterator(Config* cfg, std::nullptr_t);

    reference operator*() const;
    pointer operator->();

    // Prefix increment
    ConfigIterator& operator++();

    // Postfix increment
    ConfigIterator operator++(int) { ConfigIterator tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const ConfigIterator& a, const ConfigIterator& b);
    friend bool operator!= (const ConfigIterator& a, const ConfigIterator& b);

};
