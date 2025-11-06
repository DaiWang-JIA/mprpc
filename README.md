测试：
1../provider -i test.conf
出现：
2025-11-06 01:35:24,044:12996(0x7f28cf72fa00):ZOO_INFO@log_env@726: Client environment:zookeeper.version=zookeeper C client 3.4.10
2025-11-06 01:35:24,044:12996(0x7f28cf72fa00):ZOO_INFO@log_env@730: Client environment:host.name=ubuntu
2025-11-06 01:35:24,044:12996(0x7f28cf72fa00):ZOO_INFO@log_env@737: Client environment:os.name=Linux
2025-11-06 01:35:24,044:12996(0x7f28cf72fa00):ZOO_INFO@log_env@738: Client environment:os.arch=5.15.0-139-generic
2025-11-06 01:35:24,044:12996(0x7f28cf72fa00):ZOO_INFO@log_env@739: Client environment:os.version=#149~20.04.1-Ubuntu SMP Wed Apr 16 08:29:56 UTC 2025
2025-11-06 01:35:24,045:12996(0x7f28cf72fa00):ZOO_INFO@log_env@747: Client environment:user.name=jdw
2025-11-06 01:35:24,045:12996(0x7f28cf72fa00):ZOO_INFO@log_env@755: Client environment:user.home=/home/jdw
2025-11-06 01:35:24,045:12996(0x7f28cf72fa00):ZOO_INFO@log_env@767: Client environment:user.dir=/home/jdw/projects/mprpc/bin
2025-11-06 01:35:24,045:12996(0x7f28cf72fa00):ZOO_INFO@zookeeper_init@791: Initiating client connection, host=127.0.0.1:2181 sessionTimeout=3000 watcher=0x563167c46e8c sessionId=0 sessionPasswd=<null> context=(nil) flags=0
2025-11-06 01:35:24,046:12996(0x7f28cef09700):ZOO_INFO@check_events@1727: initiated connection to server [127.0.0.1:2181]
2025-11-06 01:35:24,157:12996(0x7f28cef09700):ZOO_INFO@check_events@1773: session establishment complete on server [127.0.0.1:2181], sessionId=0x199fa6f0b5e0003, negotiated timeout=4000
zookeeper_init success!
znode create success ...path:/FriendServiceRpc/GetFriendList
RpcProvider start service at ip:127.0.0.1 port:8000


2../consumer -i test.conf（另一个）
出现：
========================
header_size:35
rpc_header_str:
GetFriendListRpc
service_name:FriendServiceRpc
method_name:GetFriendList
args_str�
========================
2025-11-06 01:36:58,546:13259(0x7fa7b7cb4d40):ZOO_INFO@log_env@726: Client environment:zookeeper.version=zookeeper C client 3.4.10
2025-11-06 01:36:58,546:13259(0x7fa7b7cb4d40):ZOO_INFO@log_env@730: Client environment:host.name=ubuntu
2025-11-06 01:36:58,546:13259(0x7fa7b7cb4d40):ZOO_INFO@log_env@737: Client environment:os.name=Linux
2025-11-06 01:36:58,546:13259(0x7fa7b7cb4d40):ZOO_INFO@log_env@738: Client environment:os.arch=5.15.0-139-generic
2025-11-06 01:36:58,546:13259(0x7fa7b7cb4d40):ZOO_INFO@log_env@739: Client environment:os.version=#149~20.04.1-Ubuntu SMP Wed Apr 16 08:29:56 UTC 2025
2025-11-06 01:36:58,546:13259(0x7fa7b7cb4d40):ZOO_INFO@log_env@747: Client environment:user.name=jdw
2025-11-06 01:36:58,546:13259(0x7fa7b7cb4d40):ZOO_INFO@log_env@755: Client environment:user.home=/home/jdw
2025-11-06 01:36:58,546:13259(0x7fa7b7cb4d40):ZOO_INFO@log_env@767: Client environment:user.dir=/home/jdw/projects/mprpc/bin
2025-11-06 01:36:58,546:13259(0x7fa7b7cb4d40):ZOO_INFO@zookeeper_init@791: Initiating client connection, host=127.0.0.1:2181 sessionTimeout=3000 watcher=0x5580470d97ee sessionId=0 sessionPasswd=<null> context=(nil) flags=0
2025-11-06 01:36:58,547:13259(0x7fa7b7c9d700):ZOO_INFO@check_events@1727: initiated connection to server [127.0.0.1:2181]
2025-11-06 01:36:58,552:13259(0x7fa7b7c9d700):ZOO_INFO@check_events@1773: session establishment complete on server [127.0.0.1:2181], sessionId=0x199fa6f0b5e0004, negotiated timeout=4000
zookeeper_init success!
2025-11-06 01:36:58,556:13259(0x7fa7b7cb4d40):ZOO_INFO@zookeeper_close@2526: Closing zookeeper sessionId=0x199fa6f0b5e0004 to [127.0.0.1:2181]

rpc GetFriendList response success:
index:1 name:
index:2 name:gao yang
index:3 name:liu hong
index:4 name:wang shuo



3.最后第一个出现下面所示情况：
20251106 09:36:58.555503Z 12996 INFO  TcpServer::newConnection [RpcProvider] - new connection [RpcProvider-127.0.0.1:8000#1] from 127.0.0.1:33998 - TcpServer.cc:80
========================
header_size:35
rpc_header_str:
GetFriendListRpc
service_name:FriendServiceRpc
method_name:GetFriendList
args_str�
========================
do GetFriendList service!  userid:1000
20251106 09:36:58.559754Z 12996 INFO  TcpServer::removeConnectionInLoop [RpcProvider] - connection RpcProvider-127.0.0.1:8000#1 - TcpServer.cc:109
2025-11-06 01:36:59,074:12996(0x7f28cef09700):ZOO_WARN@zookeeper_interest@1570: Exceeded deadline by 124ms


