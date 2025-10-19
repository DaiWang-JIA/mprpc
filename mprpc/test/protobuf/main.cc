#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;

// int main1()
// {

//     //封装了login请求对象的数据
//     LoginRequest req;
//     req.set_name("zhang san");
//     req.set_pwd("123456");

//     //对象数据序列化 =》char *
//     std::string send_str;
//     if(req.SerializeToString(&send_str))
//     {
//         std::cout<<send_str.c_str()<<std::endl;    // zhangsan123456
//     }

//     //从send_str反序列化一个logon请求对象
//     LoginRequest reqB;
//     if(reqB.ParseFromString(send_str))  //反序列化成功
//     {
//         std::cout<<reqB.name()<<std::endl;
//         std::cout<<reqB.pwd()<<std::endl;
//     }

//     return 0;
// }

// int main2()
// {
//     //对象中还有对象
//     // LoginResponse rsp;
//     // ResultCode * rc=rsp.mutable_reslut();
//     // rc->set_errcode(0);
//     // rc->set_errmsg("登录处理失败了");


//     //列表类型
//     GetFriendListResponse rsp;
//     ResultCode * rc=rsp.mutable_reslut();
//     rc->set_errcode(0);

//     User *user1=rsp.add_friend_list();
//     user1->set_name("zhang san");
//     user1->set_age(20);
//     user1->set_sex(User::MAN);

//     User *user2=rsp.add_friend_list();
//     user2->set_name("li si");
//     user2->set_age(22);
//     user2->set_sex(User::MAN);

//     std::cout<<rsp.friend_list_size()<<std::endl;


//     for (int i = 0; i < rsp.friend_list_size(); i++) {
//         const User& user = rsp.friend_list(i);  // 获取第 i 个好友
//         std::cout << "第 " << i + 1 << " 个好友:" << std::endl;
//         std::cout << "  姓名: " << user.name() << std::endl;
//         std::cout << "  年龄: " << user.age() << std::endl;
//         std::cout << "  性别: " << (user.sex() == User::MAN ? "男" : "女") << std::endl;
//     }

//     /*
//     输出：
//     2
// 第 1 个好友:
//   姓名: zhang san
//   年龄: 20
//   性别: 男
// 第 2 个好友:
//   姓名: li si
//   年龄: 22
//   性别: 男
//     */

//     return 0;
// }



int main()
{


    return 0;
}