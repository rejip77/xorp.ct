/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'tgt-gen'.
 */

#ident "$XORP: xorp/xrl/targets/finder_base.cc,v 1.17 2003/11/18 01:47:38 hodson Exp $"


#include "finder_base.hh"


XrlFinderTargetBase::XrlFinderTargetBase(XrlCmdMap* cmds)
    : _cmds(cmds)
{
    if (_cmds)
	add_handlers();
}

XrlFinderTargetBase::~XrlFinderTargetBase()
{
    if (_cmds)
	remove_handlers();
}

bool
XrlFinderTargetBase::set_command_map(XrlCmdMap* cmds)
{
    if (_cmds == 0 && cmds) {
        _cmds = cmds;
        add_handlers();
        return true;
    }
    if (_cmds && cmds == 0) {
	remove_handlers();
        _cmds = cmds;
        return true;
    }
    return false;
}

const XrlCmdError
XrlFinderTargetBase::handle_common_0_1_get_target_name(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling common/0.1/get_target_name",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    string name;
    try {
	XrlCmdError e = common_0_1_get_target_name(
	    name);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for common/0.1/get_target_name failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("name", name);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_common_0_1_get_version(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling common/0.1/get_version",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    string version;
    try {
	XrlCmdError e = common_0_1_get_version(
	    version);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for common/0.1/get_version failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("version", version);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_common_0_1_get_status(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling common/0.1/get_status",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    uint32_t status;
    string reason;
    try {
	XrlCmdError e = common_0_1_get_status(
	    status,
	    reason);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for common/0.1/get_status failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("status", status);
	pxa_outputs->add("reason", reason);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_common_0_1_shutdown(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling common/0.1/shutdown",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = common_0_1_shutdown();
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for common/0.1/shutdown failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_register_finder_client(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 4) {
	XLOG_ERROR("Wrong number of arguments (%u != 4) handling finder/0.2/register_finder_client",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    string out_cookie;
    try {
	XrlCmdError e = finder_0_2_register_finder_client(
	    xa_inputs.get_string("instance_name"),
	    xa_inputs.get_string("class_name"),
	    xa_inputs.get_bool("singleton"),
	    xa_inputs.get_string("in_cookie"),
	    out_cookie);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/register_finder_client failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("out_cookie", out_cookie);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_unregister_finder_client(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != 1) handling finder/0.2/unregister_finder_client",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_0_2_unregister_finder_client(
	    xa_inputs.get_string("instance_name"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/unregister_finder_client failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_set_finder_client_enabled(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2) handling finder/0.2/set_finder_client_enabled",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_0_2_set_finder_client_enabled(
	    xa_inputs.get_string("instance_name"),
	    xa_inputs.get_bool("enabled"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/set_finder_client_enabled failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_finder_client_enabled(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != 1) handling finder/0.2/finder_client_enabled",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    bool enabled;
    try {
	XrlCmdError e = finder_0_2_finder_client_enabled(
	    xa_inputs.get_string("instance_name"),
	    enabled);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/finder_client_enabled failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("enabled", enabled);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_add_xrl(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 3) {
	XLOG_ERROR("Wrong number of arguments (%u != 3) handling finder/0.2/add_xrl",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    string resolved_xrl_method_name;
    try {
	XrlCmdError e = finder_0_2_add_xrl(
	    xa_inputs.get_string("xrl"),
	    xa_inputs.get_string("protocol_name"),
	    xa_inputs.get_string("protocol_args"),
	    resolved_xrl_method_name);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/add_xrl failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("resolved_xrl_method_name", resolved_xrl_method_name);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_remove_xrl(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != 1) handling finder/0.2/remove_xrl",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_0_2_remove_xrl(
	    xa_inputs.get_string("xrl"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/remove_xrl failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_resolve_xrl(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != 1) handling finder/0.2/resolve_xrl",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList resolutions;
    try {
	XrlCmdError e = finder_0_2_resolve_xrl(
	    xa_inputs.get_string("xrl"),
	    resolutions);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/resolve_xrl failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("resolutions", resolutions);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_xrl_targets(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling finder/0.2/get_xrl_targets",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList target_names;
    try {
	XrlCmdError e = finder_0_2_get_xrl_targets(
	    target_names);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/get_xrl_targets failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("target_names", target_names);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_xrls_registered_by(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != 1) handling finder/0.2/get_xrls_registered_by",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList xrls;
    try {
	XrlCmdError e = finder_0_2_get_xrls_registered_by(
	    xa_inputs.get_string("target_name"),
	    xrls);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/get_xrls_registered_by failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("xrls", xrls);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_ipv4_permitted_hosts(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling finder/0.2/get_ipv4_permitted_hosts",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList ipv4s;
    try {
	XrlCmdError e = finder_0_2_get_ipv4_permitted_hosts(
	    ipv4s);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/get_ipv4_permitted_hosts failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("ipv4s", ipv4s);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_ipv4_permitted_nets(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling finder/0.2/get_ipv4_permitted_nets",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList ipv4nets;
    try {
	XrlCmdError e = finder_0_2_get_ipv4_permitted_nets(
	    ipv4nets);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/get_ipv4_permitted_nets failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("ipv4nets", ipv4nets);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_ipv6_permitted_hosts(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling finder/0.2/get_ipv6_permitted_hosts",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList ipv6s;
    try {
	XrlCmdError e = finder_0_2_get_ipv6_permitted_hosts(
	    ipv6s);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/get_ipv6_permitted_hosts failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("ipv6s", ipv6s);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_0_2_get_ipv6_permitted_nets(const XrlArgs& xa_inputs, XrlArgs* pxa_outputs)
{
    if (xa_inputs.size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling finder/0.2/get_ipv6_permitted_nets",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    if (pxa_outputs == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    XrlAtomList ipv6nets;
    try {
	XrlCmdError e = finder_0_2_get_ipv6_permitted_nets(
	    ipv6nets);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder/0.2/get_ipv6_permitted_nets failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	pxa_outputs->add("ipv6nets", ipv6nets);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_event_notifier_0_1_register_class_event_interest(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2) handling finder_event_notifier/0.1/register_class_event_interest",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_event_notifier_0_1_register_class_event_interest(
	    xa_inputs.get_string("requester_instance"),
	    xa_inputs.get_string("class_name"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder_event_notifier/0.1/register_class_event_interest failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_event_notifier_0_1_deregister_class_event_interest(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2) handling finder_event_notifier/0.1/deregister_class_event_interest",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_event_notifier_0_1_deregister_class_event_interest(
	    xa_inputs.get_string("requester_instance"),
	    xa_inputs.get_string("class_name"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder_event_notifier/0.1/deregister_class_event_interest failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_event_notifier_0_1_register_instance_event_interest(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2) handling finder_event_notifier/0.1/register_instance_event_interest",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_event_notifier_0_1_register_instance_event_interest(
	    xa_inputs.get_string("requester_instance"),
	    xa_inputs.get_string("instance_name"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder_event_notifier/0.1/register_instance_event_interest failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlFinderTargetBase::handle_finder_event_notifier_0_1_deregister_instance_event_interest(const XrlArgs& xa_inputs, XrlArgs* /* pxa_outputs */)
{
    if (xa_inputs.size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2) handling finder_event_notifier/0.1/deregister_instance_event_interest",
            (uint32_t)xa_inputs.size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = finder_event_notifier_0_1_deregister_instance_event_interest(
	    xa_inputs.get_string("requester_instance"),
	    xa_inputs.get_string("instance_name"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for finder_event_notifier/0.1/deregister_instance_event_interest failed: %s",
            		 e.str().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

