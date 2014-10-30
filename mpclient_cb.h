#ifndef _MPCLIENT_CB_H
#define _MPCLIENT_CB_H

#include "mpclient.h"

class Monitor;

class MPClient_cb: public MPClient
{
	public:
		MPClient_cb();
		void loop();
		void set_callback_player(Monitor *mon, void (Monitor::*callback_func)());
	private:
		void (Monitor::*on_player_cb)();
		Monitor *on_player_class;
};

#endif
