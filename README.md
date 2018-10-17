### A poker c++ library
### highlights
* suport texas-holdem, pineapple
* high efficient implmentation
* well tested algorithm
* support assurance (must-have feature of cash game in china)

### who use this library
used inside Luyun Information Technologies Inc., 2012-2016. 

### why open source
* hope be helpful to anyone who wants to write a high efficient poker applications
* less bandwidth to maintain this library since 2016
* welcome any volunteers

## Dependencies
* OS: Linux (ubuntu 16.04/18.04 tested)  
* OS: MacOS 12+ 
* Compiler: g++ 4.2+
* cmake
* scons
* mysql mysql-server, mysql-client, libmysqld-dev
* redis
* libmrock
  
### install libmrock
$ git clone   git@github.com:caiqingfeng/libmrock  
$ cd libmrock  
$ make  

you need install all dependencies of libmrock first.

### Example: write a straightforward test 
	LYHoldemAlgorithmDelegate had;
	std::vector<LYCard> cards;
	LYCardHelpers::stringToCards("8s,9d,Ts,", cards);
	game->onFlop(cards);
	game->pad = (LYHoldemAlgorithmDelegate *)&had;

	st1->handCards.clear();
	st1->handCards.push_back(sA);
	st1->handCards.push_back(sK);
	st1->status = LYSeatInGame;
	st1->chipsAtHand = 0;
	seat1->gameStatus = LYSeatHoldemInAllin;
	st2->handCards.clear();
	st2->handCards.push_back(dQ);
	st2->handCards.push_back(hQ);
	st2->status = LYSeatInGame;
	st2->chipsAtHand = 0;
	seat2->gameStatus = LYSeatHoldemInAllin;

	std::vector<LYHoldemAction> actions;
	LYPotPtr pot = game->mainPot();
	pot->chipsInPot = 2000;
	pot->prepareMatchResult(game);
	pot->caculateOuts(game);
	pot->buyAssurance(LYGameFlop, st2, 100);
	pot->checkAssurance(LYGameFlop, c2);
	ASSERT_EQ(pot->chasingOuts.size(), 6+9);
	game->onTurn(c2);

# License
This library is licensed under the Apache Public License 2.0 and two clause BSD license. Please read the included LICENSE file for details.

# Patches
Anybody can contribute to libpoker development. If you are contributing a source code change, use a reasonable form: a merge request of a Git branch or a patch file attached to a ticket in Bugs tracker or sent email to me. Unless it is obvious, always state what branch or release tarball is your patch based upon.