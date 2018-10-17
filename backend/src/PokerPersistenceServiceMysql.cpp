/*
 * MysqlPersistence.cpp
 *
 *  Created on: 2013-9-11
 *      Author: chenfeng
 */

#include "PokerPersistenceServiceMysql.h"
#include "common/src/my_log.h"
#include <string>
#include <boost/lexical_cast.hpp>

int API_StringToTime(const std::string &strDateStr, int &timeData)
{
//std::cout << "source=" << strDateStr << std::endl;
    char *pBeginPos = (char*) strDateStr.c_str();
    char *pPos = strstr(pBeginPos,"-");
    if(pPos == NULL)
    {
        printf("strDateStr[%s] err \n", strDateStr.c_str());
        return -1;
    }
    int iYear = atoi(pBeginPos);
    int iMonth = atoi(pPos + 1);
    pPos = strstr(pPos + 1,"-");
    if(pPos == NULL)
    {
        printf("strDateStr[%s] err \n", strDateStr.c_str());
        return -1;
    }
    int iDay = atoi(pPos + 1);
    int iHour=0;
    int iMin=0;
    int iSec=0;
    pPos = strstr(pPos + 1," ");
    //为了兼容有些没精确到时分秒的
    if(pPos != NULL)
    {
        iHour=atoi(pPos + 1);
        pPos = strstr(pPos + 1,":");
        if(pPos != NULL)
        {
            iMin=atoi(pPos + 1);
            pPos = strstr(pPos + 1,":");
            if(pPos != NULL)
            {
                iSec=atoi(pPos + 1);
            }
        }
    }

    struct tm sourcedate;
    bzero((void*)&sourcedate,sizeof(sourcedate));
    sourcedate.tm_sec = iSec;
    sourcedate.tm_min = iMin;
    sourcedate.tm_hour = iHour;
    sourcedate.tm_mday = iDay;
    sourcedate.tm_mon = iMonth - 1;
    sourcedate.tm_year = iYear - 1900;
    timeData = mktime(&sourcedate);
    return 0;
}

int API_TimeToString(string &strDateStr,const int &timeData)
{
    char chTmp[100];
    bzero(chTmp,sizeof(chTmp));
    struct tm *p;
    time_t t = timeData;
//    p = localtime(&t);
    p = gmtime(&t);
    p->tm_year = p->tm_year + 1900;
    p->tm_mon = p->tm_mon + 1;

    snprintf(chTmp,sizeof(chTmp),"%04d-%02d-%02d %02d:%02d:%02d",
            p->tm_year, p->tm_mon, p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    strDateStr = chTmp;
    return 0;
}

PokerPersistenceServiceMysql::PokerPersistenceServiceMysql(const std::string& db_name,
					const std::string table_name,
                    const std::string profile_name,
                    const std::string poker_user_name) :
		PersistenceServiceMysql(db_name)
{
	poker_tables = table_name;
	poker_profiles = profile_name;
	poker_users = poker_user_name;
}

PokerPersistenceServiceMysql::~PokerPersistenceServiceMysql()
{
	// TODO Auto-generated destructor stub
}

void PokerPersistenceServiceMysql::clean_db()
{
}

void PokerPersistenceServiceMysql::get_tables_by_uid(const std::string& id,
		std::vector<std::string>& results)
{
	std::string statement = "select * from table_users where user_id='" + id
			+ "';";
	LY_LOG_DBG("sql statement:" << statement);
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			std::string tmp;
			tmp = res->getString("tid");
			results.push_back(tmp);
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what() << "——sql:" << statement);
	}
}

/*
 * deprecated!!!
 *
 */
void PokerPersistenceServiceMysql::enter_table(const std::string& table_id,
		const std::string& uid)
{
	std::string statement = "insert into table_users (user_id, tid) values('"
			+ uid + "','" + table_id + "');";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

/*
 * deprecated!!!
 */
unsigned int PokerPersistenceServiceMysql::leave_table(const std::string& table_id,
		const std::string& uid)
{
	std::string statement = "delete from table_users where tid='"
			+ table_id + "' and user_id='" + uid + "';";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

void PokerPersistenceServiceMysql::save_user_chips(const std::string& table_id, const std::string& uid, int chips)
{
	std::string statement = "update table_users set chips_at_hand=(" +
							std::to_string(chips) + ")" +
							" where tid='" + table_id +
							"' and user_id='" + uid + "'" +
							";";
	if (!table_have_user(table_id, uid)) {
		statement = "insert into table_users (tid, user_id, chips_at_hand) values (" +
        							table_id + "," +
        							uid + "," +
        							std::to_string(chips) + ")" +
        							";";
	}
//std::cout << statement << std::endl;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

void PokerPersistenceServiceMysql::increase_user_chips(const std::string& table_id, const std::string& uid, int chips)
{
	std::string statement = "update table_users set chips_at_hand=(" +
							std::to_string(chips) + "+chips_at_hand)" +
							" where tid='" + table_id +
							"' and user_id='" + uid + "'" +
							";";
	if (!table_have_user(table_id, uid)) {
		statement = "insert into table_users (tid, user_id, chips_at_hand) values (" +
        							table_id + "," +
        							uid + "," +
        							std::to_string(chips) + ")" +
        							";";
	}
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

bool PokerPersistenceServiceMysql::table_have_user(const std::string& table_id, const std::string& uid)
{
	std::string statement = "select * from table_users where tid='" + table_id +
							"' and user_id=(" + uid + ")" +
							";";
	bool r = false;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = true;
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

int PokerPersistenceServiceMysql::get_user_chips(const std::string& table_id, const std::string& uid)
{
	std::string statement = "select * from table_users where tid='" + table_id +
							"' and user_id=(" + uid + ")" +
							";";
	int chips = 0;
	bool table_have_user = false;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			chips = res->getInt("chips_at_hand");
			table_have_user = true;
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	if (!table_have_user) {
		int now = time(NULL);
		std::string ts;
		API_TimeToString(ts, now);
		statement = std::string("insert into table_users (tid, user_id, chips_at_hand, created_at, updated_at) values ('") +
					table_id + std::string("', ") +
					std::string("'") + uid + std::string("', ") +
					std::string("0, '") +
					ts + std::string("', '") +
					ts + std::string("');");
		try {
			sql::PreparedStatement *pstmt = con->prepareStatement(
					statement.c_str());
			sql::ResultSet *res = pstmt->executeQuery();
			delete res;
			delete pstmt;
		} catch (sql::SQLException &e) {
			LY_LOG_ERR("mysql SQLException: " << e.what());
		}
	}
	return chips;
}

void PokerPersistenceServiceMysql::increase_user_buyin(const std::string& table_id, const std::string& uid, unsigned int chips)
{
	std::string statement = "update table_users set total_buyin=(" +
							std::to_string(chips) + "+total_buyin)" +
							" where tid='" + table_id +
							"' and user_id='" + uid + "'" +
							";";
	if (!table_have_user(table_id, uid)) {
		statement = "insert into table_users (tid, user_id, total_buyin) values (" +
        							table_id + "," +
        							uid + "," +
        							std::to_string(chips) + ")" +
        							";";
	}
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

unsigned int PokerPersistenceServiceMysql::get_user_buyin(const std::string& table_id, const std::string& uid)
{
	std::string statement = "select total_buyin from table_users where tid='" + table_id +
							"' and user_id=(" + uid + ")" +
							";";
	int chips = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			chips = res->getInt("total_buyin");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return chips;
}

void PokerPersistenceServiceMysql::increase_total_buyin(const std::string& table_id, unsigned int chips)
{
	std::string statement = "update holdem_tables set total_buyin=(" +
							std::to_string(chips) + "+total_buyin)" +
							" where tid='" + table_id +
							"';";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

unsigned int PokerPersistenceServiceMysql::get_total_buyin(const std::string& table_id)
{
	std::string statement = "select total_buyin from holdem_tables where tid='" + table_id +
							"';";
	int chips = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			chips = res->getInt("total_buyin");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return chips;
}

/*
 * discarded 20160701
 */
void PokerPersistenceServiceMysql::change_insurance_income(const std::string& table_id, int chips)
{
	std::string statement = "update holdem_tables set insurance_income=(" +
							std::to_string(chips) + "+insurance_income)" +
							" where tid='" + table_id +
							"';";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

void PokerPersistenceServiceMysql::set_table_insurance_income(const std::string& table_id, int chips)
{
	std::string statement = "update holdem_tables set insurance_income=(" +
							std::to_string(chips) + ")" +
							" where tid='" + table_id +
							"';";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

int PokerPersistenceServiceMysql::get_insurance_income(const std::string& table_id)
{
	std::string statement = "select insurance_income from holdem_tables where tid='" + table_id +
							"';";
	int chips = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			chips = res->getInt("insurance_income");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return chips;
}

void PokerPersistenceServiceMysql::change_occupied_seats(const std::string& table_id, bool increase)
{
	std::string sign = "+";
	if (!increase) sign = "-";
	std::string statement = "update holdem_tables set occupied_seat_number=(occupied_seat_number" +
							sign + "1)" +
							" where tid='" + table_id +
							"';";
//std::cout << statement << std::endl;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

void PokerPersistenceServiceMysql::change_occupied_seats(const std::string& table_id, unsigned int seats)
{
	std::string statement = std::string("") +
							"update holdem_tables set occupied_seat_number=(" +
							std::to_string(seats) + ")" +
							" where tid='" + table_id +
							"';";
//std::cout << statement << std::endl;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

unsigned int PokerPersistenceServiceMysql::get_occupied_seats(const std::string& table_id)
{
	std::string statement = "select occupied_seat_number from holdem_tables where tid='" + table_id +
							"';";
	int seats = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			seats = res->getInt("occupied_seat_number");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return seats;
}

void PokerPersistenceServiceMysql::fetch_player(const std::string& uid,
		LYPlayer& player)
{
	std::string statement = "select * from users where id='" + uid + "';";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			player._id = res->getString("id");
			player.nick_name = res->getString("show_name");
			player.icon_url = res->getString("avatar");
			player.balance = res->getInt("coins");
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

void PokerPersistenceServiceMysql::update_balance(const std::string& uid,
		const std::string& game, int& delta)
{
	std::string sql = "select coins from users where id = '" + uid + "';";

	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(sql.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		int tmp = 0;
		while (res->next()) {
			tmp = res->getInt("coins");
		}
		if (tmp + delta < 0) {
			delta = -tmp;
		}
		delete res;
		delete pstmt;
		std::string statement = "update users set coins=coins+("
				+ std::to_string(delta) + ") where id='" + uid + "';";
		sql::PreparedStatement *pstmt1 = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res1 = pstmt1->executeQuery();
		delete res1;
		delete pstmt1;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

void PokerPersistenceServiceMysql::load_table(LYTable* table)
{
	std::string sql = "select * from " + poker_tables + " where tid = '" + table->_id + "';";
//std::cout << sql <<  std::endl;

	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(sql.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			table->_id = res->getString("tid");
			table->name = res->getString("name");
			table->status = (LYTableStatus)res->getInt("status");
			table->profile_id = std::to_string(res->getInt("holdem_profile_id"));
			table->server_handler = res->getString("handler");
			table->creator = std::to_string(res->getInt("creator_id"));
			table->owner = std::to_string(res->getInt("holdem_club_id"));
			table->total_seats = res->getInt("total_seats");
			table->total_buyin = res->getInt("total_buyin");
			table->game_round = res->getInt("total_games");
			table->timeout = 0; //res->getInt("timeout");
			table->assurance_flag = res->getInt("insurance_active");
			table->assurance_income = res->getInt("insurance_income");
			table->last_duration = res->getInt("last_duration");
			std::string started_at = res->getString("created_at");
			API_StringToTime(started_at, table->started_at);
			table->started_at += 3600*8; //从数据库中取到的都是UTC时间，加上东八区
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what() << " sql=" << sql);
		std::cout << "mysql SQLException: " << e.what() << " sql=" << sql;
	}
}

void PokerPersistenceServiceMysql::loadAllProfiles(
		std::vector<LYPokerProfile*> &db_profiles)
{
	std::string statement = "select * from " + poker_profiles + " ;";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			LYPokerProfile *prof = new LYPokerProfile();
			prof->_id = std::to_string(res->getInt("id"));
			prof->game_type = res->getInt("game_type");
			prof->min_chips = res->getInt("min_chips");
			prof->max_chips = res->getInt("max_chips");
			prof->small_blind = res->getInt("small_blind");
			prof->big_blind = res->getInt("big_blind");
			db_profiles.push_back(prof);
			LY_LOG_DBG("prof loaded, profile:" << prof->toString());
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

void PokerPersistenceServiceMysql::increase_table_games(const std::string& table_id)
{
	std::string sign = "+";
	std::string statement = "update holdem_tables set total_games=(total_games" +
							sign + "1)" +
							" where tid='" + table_id +
							"';";
//std::cout << statement << std::endl;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

unsigned int PokerPersistenceServiceMysql::get_table_games(const std::string& table_id)
{
	std::string statement = "select total_games from holdem_tables where tid='" + table_id +
							"';";
	int r = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = res->getInt("total_games");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

void PokerPersistenceServiceMysql::increase_user_club_games(const std::string& cid, const std::string& uid)
{
	std::string sign = "+";
	std::string statement = "update club_users set total_games=(total_games" +
							sign + "1)" +
							" where holdem_club_id='" + cid + "'" +
							" and user_id='" + uid +
							"';";
//std::cout << statement << std::endl;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

unsigned int PokerPersistenceServiceMysql::get_user_club_games(const std::string& cid, const std::string& uid)
{
	std::string statement = std::string("") + "select total_games from club_users" +
							" where holdem_club_id='" + cid + "'" +
							" and user_id='" + uid +
							"';";
	int r = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = res->getInt("total_games");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

void PokerPersistenceServiceMysql::increase_user_club_inpots(const std::string& cid, const std::string& uid)
{
	std::string sign = "+";
	std::string statement = std::string("") + "update club_users set inpot=(inpot" +
							sign + "1)" +
							" where holdem_club_id='" + cid + "'" +
							" and user_id='" + uid +
							"';";
//std::cout << statement << std::endl;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

unsigned int PokerPersistenceServiceMysql::get_user_club_inpots(const std::string& cid, const std::string& uid)
{
	std::string statement = std::string("") + "select inpot from club_users" +
							" where holdem_club_id='" + cid + "'" +
							" and user_id='" + uid +
							"';";
	int r = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = res->getInt("inpot");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

void PokerPersistenceServiceMysql::increase_user_club_wins(const std::string& cid, const std::string& uid)
{
	std::string sign = "+";
	std::string statement = std::string("") + "update club_users set winning_games=(winning_games" +
							sign + "1)" +
							" where holdem_club_id='" + cid + "'" +
							" and user_id='" + uid +
							"';";
//std::cout << statement << std::endl;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

unsigned int PokerPersistenceServiceMysql::get_user_club_wins(const std::string& cid, const std::string& uid)
{
	std::string statement = std::string("") + "select winning_games from club_users" +
							" where holdem_club_id='" + cid + "'" +
							" and user_id='" + uid +
							"';";
	int r = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = res->getInt("winning_games");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

void PokerPersistenceServiceMysql::change_user_club_income(const std::string& cid, const std::string& uid, int income)
{
	std::string statement = std::string("") + "update club_users set total_income=(" +
							std::to_string(income) + "+total_income)" +
							" where holdem_club_id='" + cid + "'" +
							" and user_id='" + uid +
							"';";
//std::cout << statement << std::endl;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

int PokerPersistenceServiceMysql::get_user_club_income(const std::string& cid, const std::string& uid)
{
	std::string statement = std::string("") + "select total_income from club_users" +
							" where holdem_club_id='" + cid + "'" +
							" and user_id='" + uid +
							"';";
	int r = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = res->getInt("total_income");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

void PokerPersistenceServiceMysql::increase_user_table_games(const std::string& table_id, const std::string& uid)
{
	std::string statement = std::string("") + "update table_users set total_games=(" +
							"1+total_games)" +
							" where tid='" + table_id +
							"' and user_id='" + uid + "'" +
							";";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

unsigned int PokerPersistenceServiceMysql::get_user_table_games(const std::string& table_id, const std::string& uid)
{
	std::string statement = std::string("") + "select total_games from table_users where tid='" + table_id +
							"' and user_id=(" + uid + ")" +
							";";
	int r = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = res->getInt("total_games");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

void PokerPersistenceServiceMysql::increase_user_table_wins(const std::string& table_id, const std::string& uid)
{
	std::string statement = std::string("") + "update table_users set winning_games=(" +
							"1+winning_games)" +
							" where tid='" + table_id +
							"' and user_id='" + uid + "'" +
							";";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

unsigned int PokerPersistenceServiceMysql::get_user_table_wins(const std::string& table_id, const std::string& uid)
{
	std::string statement = std::string("") + "select winning_games from table_users where tid='" + table_id +
							"' and user_id=(" + uid + ")" +
							";";
	int r = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = res->getInt("winning_games");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

void PokerPersistenceServiceMysql::increase_user_table_inpots(const std::string& table_id, const std::string& uid)
{
	std::string statement = std::string("") + "update table_users set inpot=(" +
							"1+inpot)" +
							" where tid='" + table_id +
							"' and user_id='" + uid + "'" +
							";";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

unsigned int PokerPersistenceServiceMysql::get_user_table_inpots(const std::string& table_id, const std::string& uid)
{
	std::string statement = std::string("") + "select inpot from table_users where tid='" + table_id +
							"' and user_id=(" + uid + ")" +
							";";
	int r = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = res->getInt("inpot");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

void PokerPersistenceServiceMysql::change_user_table_income(const std::string& table_id, const std::string& uid, int income)
{
	std::string statement = std::string("") + "update table_users set total_income=(" +
							std::to_string(income) + "+total_income), " +
							"chips_at_hand=(" + std::to_string(income) + "+chips_at_hand) "
							" where tid='" + table_id +
							"' and user_id='" + uid + "'" +
							";";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

int PokerPersistenceServiceMysql::get_user_table_income(const std::string& table_id, const std::string& uid)
{
	std::string statement = std::string("") + "select total_income from table_users where tid='" + table_id +
							"' and user_id=(" + uid + ")" +
							";";
	int r = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = res->getInt("total_income");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

std::string PokerPersistenceServiceMysql::get_club_id_by_table(const std::string& table_id)
{
	std::string statement = "select holdem_club_id from holdem_tables where tid='" + table_id +
							"';";
	std::string r = "";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = res->getString("holdem_club_id");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

void PokerPersistenceServiceMysql::increase_club_games(const std::string& club)
{
	std::string sign = "+";
	std::string statement = "update holdem_clubs set total_games=(total_games" +
							sign + "1)" +
							" where id='" + club +
							"';";
//std::cout << statement << std::endl;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}
}

unsigned int PokerPersistenceServiceMysql::get_club_games(const std::string& club)
{
	std::string statement = "select total_games from holdem_clubs where id='" + club +
							"';";
	int r = 0;
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		while (res->next()) {
			r = res->getInt("total_games");
			break;
		}
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

	return r;
}

void PokerPersistenceServiceMysql::increase_user_chips_buyin_to_db(const std::string& table_id, const std::string& uid,
					int chips) //更新用户的chips_at_hand, buyin, mysql
{
//insert a record into table_buyins
	int now = time(NULL);
	std::string ts;
	API_TimeToString(ts, now);

	std::string sql = std::string("insert into table_buyins")  +
					std::string(" (tid, user_id, buyin, created_at, updated_at) values('") +
					table_id + "'," +
					uid + "," +
					std::to_string(chips) + ", '" +
					ts + std::string("', '") +
					ts +
					std::string("');")
					;

	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(sql.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
//	std::cout << e.what() << sql;
		LY_LOG_ERR("mysql SQLException: " << e.what() << " sql=" << sql);
	}

//update table_user's chips and buyin
	std::string statement = std::string("") +
							"update table_users set total_buyin=(" +
							std::to_string(chips) + "+total_buyin)" + "," +
							"chips_at_hand=(" + std::to_string(chips) + "+chips_at_hand)" +
							" where tid='" + table_id +
							"' and user_id='" + uid + "'" +
							";";
	try {
		sql::PreparedStatement *pstmt = con->prepareStatement(
				statement.c_str());
		sql::ResultSet *res = pstmt->executeQuery();
		delete res;
		delete pstmt;
	} catch (sql::SQLException &e) {
		LY_LOG_ERR("mysql SQLException: " << e.what());
	}

//增加table total_buyin
	increase_total_buyin(table_id, chips);
}

