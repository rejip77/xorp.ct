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

// $XORP: xorp/bgp/harness/command.hh,v 1.5 2003/06/19 00:46:09 hodson Exp $

#ifndef __BGP_HARNESS_COMMAND_HH__
#define __BGP_HARNESS_COMMAND_HH__

#include "libxorp/ref_ptr.hh"
#include "peer.hh"

class TimeVal;

/*
** All the commands to the coordinating process come via this class.
** The commands are delivered either via XRL's or from a file.
*/

class Command {
public:
    Command(EventLoop& eventloop, XrlRouter& xrlrouter);

    /*
    ** Load command map.
    */
    void load_command_map();

    /*
    ** Accept a command for the coordinating process.
    */
    void command(const string& line) throw(InvalidString);

    /*
    ** Test to see if there are any uncompleted commands.
    */
    bool pending();

    /*
    ** Data from the test peers.
    */
    void datain(const string&  peer, const bool& status, const TimeVal& tv,
		const vector<uint8_t>&  data);
    void datain_error(const string&  peer, const string& reason);
    void datain_closed(const string&  peer);
   
    /*
    ** All commands to peers sent through here.
    */
    void peer(const string& line, const vector<string>& v) 
	throw(InvalidString);

    void reset(const string& line, const vector<string>& v);
    void target(const string& line, const vector<string>& v) 
	throw(InvalidString);
    void initialise(const string& line, const vector<string>& v) 
	throw(InvalidString);
    void initialise_callback(const XrlError& error, string peername);
private:
    EventLoop& _eventloop;
    XrlRouter& _xrlrouter;

    uint32_t _init_count;	// Number of initialisations with
				// test_peers currently in progress.

    /*
    ** Supported commands.
    */
    typedef void (Command::* MEMFUN)(const string& line,
				     const vector<string>& v);

    struct PCmd {
	PCmd(const Command::MEMFUN& mem_fun) : _mem_fun(mem_fun)
	{}

	void dispatch(Command* c, const string& line, const vector<string>&v)
	{
	    (c->*_mem_fun)(line, v);
	}
	Command::MEMFUN _mem_fun;
    };

    typedef map<const string, PCmd> StringCommandMap;
    StringCommandMap _commands;

    typedef map<const string, ref_ptr<Peer> > NamePeerMap;
    NamePeerMap _peers;

    string _target_hostname;
    string _target_port;
};

#endif // __BGP_HARNESS_COMMAND_HH__
