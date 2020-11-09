﻿#include "mainwindow.h"
#include "login_dialog.h"
#include <QApplication>

#include "xtp_trader_api.h"
#include <string>
#include <map>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32

#include "trade_spi.h"
#include "FileUtils.h"
#include "xtp_quote_api.h"
#include "quote_spi.h"

//XTP::API::TraderApi *pUserApi;
//bool is_connected_ = false;
//std::string trade_server_ip;
//int trade_server_port;
//uint64_t session_id_ = 0;
//std::map<uint64_t, uint64_t> map_session;
//uint64_t *session_arrary = NULL;
//FileUtils *fileUtils = new FileUtils();
//XTPOrderInsertInfo *orderList = NULL;
//std::string quote_server_ip;
//int quote_server_port;
//std::string quote_username;
//std::string quote_password;
//XTP_PROTOCOL_TYPE quote_protocol = XTP_PROTOCOL_UDP;

//int test()
//{
//    FileUtils *fileUtils = new FileUtils();
//    if (!fileUtils->init())
//    {
//        std::cout << "The config.json file parse error." << std::endl;
//#ifdef _WIN32
//        system("pause");
//#endif

//        return 0;
//    }

//    //读取交易配置
//    trade_server_ip = fileUtils->stdStringForKey("trade_ip");
//    trade_server_port = fileUtils->intForKey("trade_port");
//    int out_count = fileUtils->intForKey("out_count");
//    bool auto_save = fileUtils->boolForKey("auto_save");
//    int client_id = fileUtils->intForKey("client_id");
//    int account_count = fileUtils->countForKey("account");
//    int resume_type = fileUtils->intForKey("resume_type");
//    std::string account_key = fileUtils->stdStringForKey("account_key");
//#ifdef _WIN32
//    std::string filepath = fileUtils->stdStringForKey("path");
//#else
//    std::string filepath = fileUtils->stdStringForKey("path_linux");
//#endif // _WIN32

//    //读取行情配置
//    quote_server_ip = fileUtils->stdStringForKey("quote_ip");
//    quote_server_port = fileUtils->intForKey("quote_port");
//    quote_username = fileUtils->stdStringForKey("quote_user");
//    quote_password = fileUtils->stdStringForKey("quote_password");
//    quote_protocol = (XTP_PROTOCOL_TYPE)(fileUtils->intForKey("quote_protocol"));
//    int32_t quote_buffer_size = fileUtils->intForKey("quote_buffer_size");

//    //读取心跳超时配置
//    int32_t heat_beat_interval = fileUtils->intForKey("hb_interval");

//    //初始化行情api
//    XTP::API::QuoteApi *pQuoteApi = XTP::API::QuoteApi::CreateQuoteApi(client_id, filepath.c_str(), XTP_LOG_LEVEL_DEBUG); //log日志级别可以调整
//    MyQuoteSpi *pQuoteSpi = new MyQuoteSpi();
//    pQuoteApi->RegisterSpi(pQuoteSpi);

//    //设定行情服务器超时时间，单位为秒
//    pQuoteApi->SetHeartBeatInterval(heat_beat_interval); //此为1.1.16新增接口
//    //设定行情本地缓存大小，单位为MB
//    pQuoteApi->SetUDPBufferSize(quote_buffer_size); //此为1.1.16新增接口

//    int loginResult_quote = -1;
//    //登录行情服务器,自1.1.16开始，行情服务器支持UDP连接，推荐使用UDP
//    loginResult_quote = pQuoteApi->Login(quote_server_ip.c_str(), quote_server_port, quote_username.c_str(), quote_password.c_str(), quote_protocol);
//    if (loginResult_quote == 0)
//    {
//        //登录行情服务器成功后，订阅行情
//        int instrument_count = fileUtils->countForKey("quote_ticker.instrument");
//        int quote_exchange = fileUtils->intForKey("quote_ticker.exchange");

//        //从配置文件中读取需要订阅的股票
//        char **allInstruments = new char *[instrument_count];
//        for (int i = 0; i < instrument_count; i++)
//        {
//            allInstruments[i] = new char[7];
//            std::string instrument = fileUtils->stdStringForKey("quote_ticker.instrument[%d]", i);
//            strcpy(allInstruments[i], instrument.c_str());
//        }

//        //开始订阅,注意公网测试环境仅支持TCP方式，如果使用UDP方式会没有行情数据，实盘大多数使用UDP连接
//        pQuoteApi->SubscribeMarketData(allInstruments, instrument_count, (XTP_EXCHANGE_TYPE)quote_exchange);
////        pQuoteApi->QueryAllTickers((XTP_EXCHANGE_TYPE)quote_exchange);
////        pQuoteApi->QueryAllTickersFullInfo((XTP_EXCHANGE_TYPE)quote_exchange);
//        pQuoteApi->SubscribeAllOrderBook((XTP_EXCHANGE_TYPE)quote_exchange);

//        //释放
//        for (int i = 0; i < instrument_count; i++)
//        {
//            delete[] allInstruments[i];
//            allInstruments[i] = NULL;
//        }

//        delete[] allInstruments;
//        allInstruments = NULL;
//    }
//    else
//    {
//        //登录失败，获取失败原因
//        XTPRI *error_info = pQuoteApi->GetApiLastError();
//        std::cout << "Login to server error, " << error_info->error_id << " : " << error_info->error_msg << std::endl;
//    }

//    if (account_count > 0)
//    {
//        //针对多用户的情况
//        orderList = new XTPOrderInsertInfo[account_count];
//        memset(orderList, 0, sizeof(XTPOrderInsertInfo) * account_count);
//    }

//    //初始化交易类Api
//    pUserApi = XTP::API::TraderApi::CreateTraderApi(client_id, filepath.c_str(), XTP_LOG_LEVEL_DEBUG); // 创建UserApi，log日志级别可以调整
//    pUserApi->SubscribePublicTopic((XTP_TE_RESUME_TYPE)resume_type);
//    pUserApi->SetSoftwareVersion("1.1.0");				//设定此软件的开发版本号，用户自定义
//    pUserApi->SetSoftwareKey(account_key.c_str());		//设定用户的开发代码，在XTP申请开户时，由xtp人员提供
//    pUserApi->SetHeartBeatInterval(heat_beat_interval); //设定交易服务器超时时间，单位为秒，此为1.1.16新增接口
//    MyTraderSpi *pUserSpi = new MyTraderSpi();
//    pUserApi->RegisterSpi(pUserSpi); // 注册事件类
//    pUserSpi->setUserAPI(pUserApi);
//    pUserSpi->set_save_to_file(auto_save);
//    if (out_count > 0)
//    {
//        pUserSpi->OutCount(out_count);
//    }
//    else
//    {
//        out_count = 1;
//    }

//    if (account_count > 0)
//    {
//        //多用户时，用session数组来管理用户session_id
//        session_arrary = new uint64_t[account_count];

//        for (int i = 0; i < account_count; i++)
//        {
//            //从配置文件中读取第i个用户登录信息
//            std::string account_name = fileUtils->stdStringForKey("account[%d].user", i);
//            std::string account_pw = fileUtils->stdStringForKey("account[%d].password", i);

//            uint64_t temp_session_ = 0;
//            std::cout << account_name << " login begin." << std::endl;
//            temp_session_ = pUserApi->Login(trade_server_ip.c_str(), trade_server_port, account_name.c_str(), account_pw.c_str(), XTP_PROTOCOL_TCP); //登录交易服务器

//            if (session_id_ == 0)
//            {
//                session_id_ = temp_session_;
//            }

//            if (temp_session_ > 0)
//            {
//                //登录成功后，建立session_id与用户之间的映射关系
//                map_session.insert(std::make_pair(temp_session_, i));
//            }
//            else
//            {
//                //登录失败，获取登录失败原因
//                XTPRI *error_info = pUserApi->GetApiLastError();
//                std::cout << account_name << " login to server error, " << error_info->error_id << " : " << error_info->error_msg << std::endl;
//            }

//            session_arrary[i] = temp_session_;
//        }
//    }

//    delete fileUtils;
//    fileUtils = NULL;

//    if (orderList)
//    {
//        delete[] orderList;
//        orderList = NULL;
//    }

//    if (session_arrary)
//    {
//        delete[] session_arrary;
//        session_arrary = NULL;
//    }

//    return 0;
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setStyleSheet("QTableCornerButton::section{background:rgb(30,30,30);}");
    // test();
    //return 0;
    return a.exec();
}