//
//  GameSettings.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "metaLib.hpp"
#include <vector>
#include <bitset>
#include <array>
#include "Container.hpp"


template<typename Systems>
struct GameSettings {
    
    constexpr static size_t NumberOfSystems = Systems{}.size();
    
    using AllComponentsInSystems = meta::as_list<decltype(meta::FindComponents<Systems>{}.Iterate())>;
    using UniqueComponents = meta::unique<AllComponentsInSystems>;
    
    
    template <typename... Ts>
    using TupleOfContainers = std::tuple<Container<Ts>...>;
    
    using SystemsTuple = meta::mp_rename<Systems, std::tuple>;
    using UpdateSystems = meta::filter<Systems, meta::HasUpdateFunction>;
    using UpdateSystemsTuple = meta::mp_rename<UpdateSystems, std::tuple>;
    
    using AllComponents = meta::mp_rename<UniqueComponents, TupleOfContainers>;
    
    using Bitset = std::bitset<UniqueComponents{}.size()>;
    using SystemBitsets = std::array<Bitset, NumberOfSystems>;
    
    using ComponentSystems = meta::as_list<decltype(meta::FindComponentSystems<Systems, UniqueComponents>{}.Iterate())>;
    using ComponentSystemsTuple = meta::mp_rename<ComponentSystems, std::tuple>;
    
    template<typename System>
    constexpr static size_t GetSystemID() {
        static_assert(meta::find_index<Systems, System> {} != meta::npos{}, "System is not valid");
        return meta::find_index<Systems, System> {};
    }
    
    template<typename Component>
    constexpr static size_t GetComponentID() {
        static_assert(meta::find_index<UniqueComponents, Component> {} != meta::npos{}, "Component is not valid");
        return meta::find_index<UniqueComponents, Component> {};
    }
};


struct DefaulSettings {
    using Settings = GameSettings<meta::list<int, double, float>>;
};
