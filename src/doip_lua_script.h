#ifndef DOIP_LUA_SCRIPT_H
#define DOIP_LUA_SCRIPT_H

#include "selene.h"
#include "utilities.h"
#include <string>
#include <stdio.h>

constexpr char VIN[] = "VIN";
constexpr char LA[] = "LOGICAL_ADDRESS";
constexpr char EID[] = "EID";
constexpr char GID[] = "GID";
constexpr char FA[] = "FURTHER_ACTION";

class DoipLuaScript 
{
public:
    DoipLuaScript(const std::string& luaScript);
    
    std::string getVin() const;
    std::uint64_t getGid() const;
    std::uint64_t getEid() const;
    std::uint16_t getLogicalAddress() const;
    std::uint8_t getFurtherAction() const;
private:
    sel::State lua_state;
    
    std::string vin_;
    std::uint64_t eid_;
    std::uint64_t gid_;
    std::uint16_t logicalAddress_;
    std::uint8_t furtherAction_;
    
    bool EIDflag = false;
};

#endif /* DOIP_LUA_SCRIPT_H */

