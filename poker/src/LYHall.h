/*
 * LYHall.h
 *
 *  Created on: 2013-6-27
 *      Author: caiqingfeng
 */

#ifndef LYHALL_H_
#define LYHALL_H_

#include <map>
#include <string>
#include <vector>
#include "LYTable.h"
#include "LYPlayerMgr.h"

/*
 * Management Class of LYTable
 */
using namespace std;

class LYHall {
protected:
	LYPlayerMgr *playerMgr;
	std::string serverHandler;

protected:
    std::map<std::string, LYTablePtr> tables; // id ,
    // client user create

public:
    LYHall(LYPlayerMgr *pm);
    void reset();

public:
//    static LYHall *Instance();
	virtual ~LYHall();

	/*
	 * createTable是通用创建桌子，不会被外面类来调用
	 * createHoldemTable是专门创建HoldemTable的，服务器和客户端代码是一样，但为了在Poker这个库中不对Holdem产生依赖
	 * 这个函数定义成虚函数，ServerHall/ClientHall都要实现这个方法
	 */
    void createTable(const string &table_id, const unsigned int ts,
    		enum LYTableStatus sta, const string &tbl_name);
    void closeTable(const string &table_id);
    void createTable(const string &table_id, LYTablePtr tbl);
    virtual void createHoldemTable(const string &table_id, const unsigned int ts,
    		enum LYTableStatus sta, const string &tbl_name) {};

public:
    std::vector<LYTablePtr> fetchTablesByName(const std::string &name);
    LYTablePtr fetchTableById(const std::string &tid);
    std::map<std::string, LYTablePtr> &getAllTables(){ return tables; };
//
    inline bool haveTable(const string& table_id)
    {
        return tables.find(table_id) != tables.end();
    }
    inline unsigned int getTablesSize() {return tables.size();};

};

#endif /* LYHall_H_ */
