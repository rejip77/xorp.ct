/*
 * Copyright (c) 2002 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by '../scripts/tgt-gen'.
 *
 * $XORP: xorp/xrl/targets/finder_base.hh,v 1.5 2003/02/06 22:21:31 hodson Exp $
 */


#ifndef __XRL_INTERFACES_FINDER_BASE_HH__
#define __XRL_INTERFACES_FINDER_BASE_HH__

#undef XORP_LIBRARY_NAME
#define XORP_LIBRARY_NAME "XrlFinderTarget"

#include "libxorp/xlog.h"
#include "libxipc/xrl_cmd_map.hh"

class XrlFinderTargetBase {
protected:
    XrlCmdMap* _cmds;
    
public:
    XrlFinderTargetBase(XrlCmdMap* cmds) : _cmds(cmds) { add_handlers(); }
    virtual ~XrlFinderTargetBase() { remove_handlers(); }

    inline const string& name() const { return _cmds->name(); }
    inline const char* version() const { return "finder/0.0"; }

protected:

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Get name of Xrl Target
     */
    virtual XrlCmdError common_0_1_get_target_name(
	// Output values, 
	string&	name) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Get version string from Xrl Target
     */
    virtual XrlCmdError common_0_1_get_version(
	// Output values, 
	string&	version) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Fails if target_name is already registered. The target_name must
     *  support the finder_client interface in order to be able to process
     *  messages from the finder.
     */
    virtual XrlCmdError finder_0_1_register_finder_client(
	// Input values, 
	const string&	target_name, 
	const string&	class_name, 
	const string&	in_cookie, 
	// Output values, 
	string&	out_cookie) = 0;

    virtual XrlCmdError finder_0_1_unregister_finder_client(
	// Input values, 
	const string&	target_name) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Add resolved Xrl into system, fails if xrl is already registered.
     */
    virtual XrlCmdError finder_0_1_add_xrl(
	// Input values, 
	const string&	xrl, 
	const string&	protocol_name, 
	const string&	protocol_args, 
	// Output values, 
	string&	resolved_xrl_method_name) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Remove xrl
     */
    virtual XrlCmdError finder_0_1_remove_xrl(
	// Input values, 
	const string&	xrl) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Resolve Xrl
     */
    virtual XrlCmdError finder_0_1_resolve_xrl(
	// Input values, 
	const string&	xrl, 
	// Output values, 
	XrlAtomList&	resolutions) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Get list of registered Xrl targets
     */
    virtual XrlCmdError finder_0_1_get_xrl_targets(
	// Output values, 
	XrlAtomList&	target_names) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Get list of Xrls registered by target
     */
    virtual XrlCmdError finder_0_1_get_xrls_registered_by(
	// Input values, 
	const string&	target_name, 
	// Output values, 
	XrlAtomList&	xrls) = 0;

private:
    const XrlCmdError handle_common_0_1_get_target_name(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_common_0_1_get_version(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_finder_0_1_register_finder_client(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_finder_0_1_unregister_finder_client(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_finder_0_1_add_xrl(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_finder_0_1_remove_xrl(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_finder_0_1_resolve_xrl(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_finder_0_1_get_xrl_targets(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_finder_0_1_get_xrls_registered_by(const Xrl& x, XrlArgs* a);

    void add_handlers()
    {
	if (_cmds->add_handler("common/0.1/get_target_name", 
	    callback(this, &XrlFinderTargetBase::handle_common_0_1_get_target_name)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://finder/common/0.1/get_target_name");
	}
	if (_cmds->add_handler("common/0.1/get_version", 
	    callback(this, &XrlFinderTargetBase::handle_common_0_1_get_version)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://finder/common/0.1/get_version");
	}
	if (_cmds->add_handler("finder/0.1/register_finder_client", 
	    callback(this, &XrlFinderTargetBase::handle_finder_0_1_register_finder_client)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://finder/finder/0.1/register_finder_client");
	}
	if (_cmds->add_handler("finder/0.1/unregister_finder_client", 
	    callback(this, &XrlFinderTargetBase::handle_finder_0_1_unregister_finder_client)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://finder/finder/0.1/unregister_finder_client");
	}
	if (_cmds->add_handler("finder/0.1/add_xrl", 
	    callback(this, &XrlFinderTargetBase::handle_finder_0_1_add_xrl)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://finder/finder/0.1/add_xrl");
	}
	if (_cmds->add_handler("finder/0.1/remove_xrl", 
	    callback(this, &XrlFinderTargetBase::handle_finder_0_1_remove_xrl)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://finder/finder/0.1/remove_xrl");
	}
	if (_cmds->add_handler("finder/0.1/resolve_xrl", 
	    callback(this, &XrlFinderTargetBase::handle_finder_0_1_resolve_xrl)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://finder/finder/0.1/resolve_xrl");
	}
	if (_cmds->add_handler("finder/0.1/get_xrl_targets", 
	    callback(this, &XrlFinderTargetBase::handle_finder_0_1_get_xrl_targets)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://finder/finder/0.1/get_xrl_targets");
	}
	if (_cmds->add_handler("finder/0.1/get_xrls_registered_by", 
	    callback(this, &XrlFinderTargetBase::handle_finder_0_1_get_xrls_registered_by)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://finder/finder/0.1/get_xrls_registered_by");
	}
    }

    void remove_handlers()
    {
	_cmds->remove_handler("common/0.1/get_target_name");
	_cmds->remove_handler("common/0.1/get_version");
	_cmds->remove_handler("finder/0.1/register_finder_client");
	_cmds->remove_handler("finder/0.1/unregister_finder_client");
	_cmds->remove_handler("finder/0.1/add_xrl");
	_cmds->remove_handler("finder/0.1/remove_xrl");
	_cmds->remove_handler("finder/0.1/resolve_xrl");
	_cmds->remove_handler("finder/0.1/get_xrl_targets");
	_cmds->remove_handler("finder/0.1/get_xrls_registered_by");
    }
};

#endif /* __XRL_INTERFACES_FINDER_BASE_HH__ */
