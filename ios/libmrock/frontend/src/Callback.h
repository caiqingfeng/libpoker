/*
 * ViewCallback.h
 *
 *  Created on: 2013-7-25
 *      Author: chenfeng
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

using namespace std;

namespace com {
namespace luyun {
namespace msg {
namespace client {

class Callback {
public:
    Callback();
    virtual ~Callback();

//    virtual void onPlainText(string& text) = 0;
    //virtual void onCheckin(Checkin& checkin) = 0;
    //virtual void onTrans(const string& trans) = 0;
};

} /* namespace client */
} /* namespace msg */
} /* namespace luyun */
} /* namespace com */

#endif /* CALLBACK_H_ */
