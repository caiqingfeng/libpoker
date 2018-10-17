/*
 * LYTimer.h
 *
 *  Created on: 2013-5-3
 *      Author: caiqingfeng
 */

#ifndef LYTIMER_H_
#define LYTIMER_H_

#include <boost/shared_ptr.hpp>
#include <string>
//#include <ev.h>
#include "timer.pb.h"

struct ev_timer;

using com::luyun::timer::shared::LYTimerAction;

class LYTimer {
public:
	LYTimer();
	LYTimer(const std::string &cli, float af, float re);
	virtual ~LYTimer();

public:
	std::string get_server_timer_id() {return server_timer_id;};
	void set_server_timer_id(const std::string &val) {server_timer_id = val;};
	enum LYTimerAction get_action() {return action;};
	void set_action(enum LYTimerAction act) {action = act; };
//	boost::shared_ptr<ev_timer> getTimeout() {return time_out;};
	boost::shared_ptr<long > getTimeout() {return time_out;};
	std::string getClient() {return client;};
	void setClient(const std::string &cli) {client = cli;};
	std::string get_client_timer_id() {return client_timer_id;};
	std::string get_second_client_timer_id() {return second_client_timer_id;};
	void set_client_timer_id(const std::string &val) {client_timer_id=val;};
	void set_second_client_timer_id(const std::string &val) {second_client_timer_id=val;};
	void onTimeout();
	void set_client_reserved1(int val) {client_reserved1 = val;};
	void set_client_reserved2(int val) {client_reserved2 = val;};
	void set_client_reserved3(int val) {client_reserved3 = val;};
	void set_client_reserved4(int val) {client_reserved4 = val;};
	void set_client_reserved5(int val) {client_reserved5 = val;};
	int get_client_reserved1() {return client_reserved1;};
	int get_client_reserved2() {return client_reserved2;};
	int get_client_reserved3() {return client_reserved3;};
	int get_client_reserved4() {return client_reserved4;};
	int get_client_reserved5() {return client_reserved5;};
	void set_after(const float val) {after = val;};
	float get_after() {return after;};
	void set_repeat(const float val) {repeat = val;};
	float get_repeat() {return repeat;};
	void set_will_stop_at(const time_t val) {will_stop_at = val;};
	time_t get_will_stop_at() {return will_stop_at;};
	bool go_fire(time_t now);
	void set_next_fire_at(time_t t);

private:
	std::string server_timer_id; //got from server side
	enum LYTimerAction action;
	std::string client; //get it from zmq_id
	std::string client_timer_id; // client side timer id, in poker, it's table id
	std::string second_client_timer_id; //second level timer id
	float after;
	float repeat;
	time_t next_fire_at;
	time_t will_stop_at;
	int client_reserved1;
	int client_reserved2;
	int client_reserved3;
	int client_reserved4;
	int client_reserved5;
//	boost::shared_ptr<ev_timer> time_out;
	boost::shared_ptr<long > time_out;

public:
	std::string toString();
};

typedef boost::shared_ptr<LYTimer> LYTimerPtr;

#endif /* LYTIMER_H_ */
