// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2003 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

// $XORP: xorp/libfeaclient/xrl_ifmgr_mirror.hh,v 1.2 2003/08/28 00:08:55 hodson Exp $

#ifndef __LIBFEACLIENT_XRL_IFMGR_MIRROR_HH__
#define __LIBFEACLIENT_XRL_IFMGR_MIRROR_HH__

#include "config.h"

#include "xrl/targets/fea_ifmgr_mirror_base.hh"
#include "ifmgr_atoms.hh"
#include "ifmgr_cmd_queue.hh"

class XrlStdRouter;
class EventLoop;
class IfMgrCommandBase;
class XrlIfMgrMirrorRouter;
class XrlIfMgrMirrorTarget;

/**
 * @short Base for classes that watch for Finder events from an
 * XrlIfMgrMirrorRouter.
 */
class XrlIfMgrMirrorRouterObserver {
public:
    virtual ~XrlIfMgrMirrorRouterObserver() = 0;
    virtual void finder_disconnect_event() = 0;
    virtual void finder_ready_event() = 0;
};

/**
 * @short Base for classes that are interested in configuration event
 * hint commands.
 */
class IfMgrHintObserver {
public:
    virtual ~IfMgrHintObserver() = 0;
    virtual void tree_complete() = 0;
    virtual void updates_made() = 0;
};

/**
 * @short Maintainer of a local mirror of central IfMgr configuration
 * state via Xrls sent by the IfMgr.
 */
class XrlIfMgrMirror
    : protected XrlIfMgrMirrorRouterObserver, protected IfMgrHintObserver
{
public:
    typedef IfMgrCommandSinkBase::Cmd Cmd;

    enum Status {
	NO_FINDER,
	REGISTERING_WITH_FEA,
	WAITING_FOR_TREE_IMAGE,
	READY
    };

    /**
     * Default Xrl Target to register interest with.
     */
    static const char* DEFAULT_REGISTRATION_TARGET;

public:
    /**
     * Constructor
     *
     * @param eventloop to use for events.
     * @param reg_tgt name of Xrl class or target to supply interface
     * configuration updates.
     */
    XrlIfMgrMirror(EventLoop&	eventloop,
		   const char*	reg_tgt = DEFAULT_REGISTRATION_TARGET);

    /**
     * Constructor
     *
     * @param eventloop to use for events.
     * @param finder_addr address to route finder messages to.
     * @param reg_tgt name of Xrl class or target to supply interface
     * configuration updates.
     */
    XrlIfMgrMirror(EventLoop&	eventloop,
		   IPv4		finder_addr,
		   const char*	reg_tgt = DEFAULT_REGISTRATION_TARGET);

    /**
     * Constructor
     *
     * @param eventloop to use for events.
     * @param finder_addr address to route finder messages to.
     * @param finder_port port to direct finder messages to.
     * @param reg_tgt name of Xrl class or target to supply interface
     *                configuration updates.
     */
    XrlIfMgrMirror(EventLoop&	e,
		   IPv4		finder_addr,
		   uint16_t	finder_port,
		   const char*	reg_tgt = DEFAULT_REGISTRATION_TARGET);

    ~XrlIfMgrMirror();

    inline const IfMgrIfTree& iftree() const		{ return _iftree; }

    Status status() const;

    /**
     * Attach an observer interested in receiving IfMgr hints.
     * @param o observer to be attached.
     * @return true on success, false if observer is already registered.
     */
    bool attach_hint_observer(IfMgrHintObserver* o);

    /**
     * Detach a party interested in receiving IfMgr hints.
     * @param o observer to be detached.
     * @return true on success, false if observer was not registered.
     */
    bool detach_hint_observer(IfMgrHintObserver* o);

protected:
    void finder_ready_event();
    void finder_disconnect_event();
    void register_with_ifmgr();

protected:
    void tree_complete();
    void updates_made();

protected:
    void set_status(Status s);
    void register_cb(const XrlError& e);

protected:
    EventLoop&			_e;
    XrlIfMgrMirrorRouter*	_rtr;
    IfMgrIfTree	   		_iftree;
    IfMgrCommandDispatcher	_dispatcher;
    XrlIfMgrMirrorTarget*	_xrl_tgt;
    string			_rtarget;	// registration target (ifmgr)
    Status			_status;
    list<IfMgrHintObserver*>	_hint_observers;

    XorpTimer			_reg_timer;	// registration timer
};

#endif // __LIBFEACLIENT_XRL_IFMGR_MIRROR_HH__
