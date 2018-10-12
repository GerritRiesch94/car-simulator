#include "doip_lua_script.h"

/**
 * Constructor. Reads lua file and save fields. 
 * @param luaScript     path to the lua script file
 */
DoipLuaScript::DoipLuaScript(const std::string& luaScript) {

    const std::string id = "Main";
    
    if(utils::existsFile(luaScript)) {
        lua_state.Load(luaScript);
        
        if(lua_state[id.c_str()].exists()) {
            
            std::string vin = lua_state[id.c_str()][VIN];
            if(!vin.empty()) {
                //set vin from lua file
                vin_ = vin;
            } else {
                //set vin to default
                vin_ = "00000000000000000";
            }
            
            auto logicaladdress = lua_state[id.c_str()][LA];
            if(logicaladdress.exists()) {
                //set la from lua
                logicalAddress_ = int(logicaladdress);
            } else {
                //set la to default
                logicalAddress_ = int(0x0000);
            }
            
            std::string eid = lua_state[id.c_str()][EID];
            if(!eid.empty()) {
                //set eid from lua              
                eid_ = std::stoull(eid);       
            } else {
                
                EIDflag = true;     
            }
            
            std::string gid = lua_state[id.c_str()][GID];
            if(!gid.empty()) {
                //set gid from lua
                gid_ = std::stoull(gid);
            } else {
                //set gid to default
                gid_ = 0x000000000000;
            }
            
            auto furtheraction = lua_state[id.c_str()][FA];
            if(furtheraction.exists()) {
                //set furtheraction from lua
                furtherAction_ = int(furtheraction);
            } else {
                //set furtheraction to default
                furtherAction_ = int(0x00);
            }
            
        } else {
            throw std::invalid_argument("Invalid Lua configuration file for doip simulation");
        }
    } else {
        throw std::invalid_argument("No Lua configuration file for doip simulation found");
    }
}


/**
 * Gets the VIN from the lua configuration file
 * @return      vin as string
 */
std::string DoipLuaScript::getVin() const 
{
    return vin_;
}

/**
 * Gets the Logical Address from the lua configuration file
 * @return      logical address
 */
std::uint16_t DoipLuaScript::getLogicalAddress() const
{
    return logicalAddress_;
}


/**
 * Gets the EID from the lua configuration file
 * @return      eid as string
 */
unsigned long DoipLuaScript::getEid() const
{
    return eid_;
}

/**
 * Gets the GID from the lua configuration file
 * @return      gid as string
 */
unsigned long DoipLuaScript::getGid() const 
{
    return gid_;
}

/**
 * Gets the Further Action Code from the lua configuration file
 * @return      further action code
 */
std::uint8_t DoipLuaScript::getFurtherAction() const
{
    return furtherAction_;
}