$!  COMPILE.COM
$ SET NOON
$! CC_DEF = "''P1'"
$ CC_DEF = "/STANDARD=VAXC/NOMEMBER_ALIGN"
$ SET VERIFY
$ CC'CC_DEF'/NOLIST MAIN
$ CC'CC_DEF'/NOLIST VMS
$ CC'CC_DEF'/NOLIST VMS_IO
$ CC'CC_DEF'/NOLIST io
$ CC'CC_DEF'/NOLIST protocol
$ CC'CC_DEF'/NOLIST FILE_QUEUE
$ CC'CC_DEF'/NOLIST headers
$ CC'CC_DEF'/NOLIST bcb_crc
$ CC'CC_DEF'/NOLIST send_file
$ CC'CC_DEF'/NOLIST RECV_FILE
$ CC'CC_DEF'/NOLIST VMS_SEARCH_ROUTE
$ CC'CC_DEF'/NOLIST VMS_DECNET
$ CC'CC_DEF'/NOLIST util
$ CC'CC_DEF'/NOLIST NMR
$ CC'CC_DEF'/NOLIST VMS_TCP
$ CC'CC_DEF'/NOLIST READ_CONFIG
$ CC'CC_DEF'/NOLIST GONE_SERVER
$! CC'CC_DEF'/NOLIST CLUSTER_SERVER
$ CC'CC_DEF'/NOLIST MX_TO_NJE
$ CC'CC_DEF'/NOLIST YGONE
$! SET DEF [MAILER_SRC.BMAIL]
$! CC'CC_DEF'/NOLIST/DEFINE=YGONE MAIL_CHECK
$! SET DEF [NJE_SRC]
$ SET NOVERIFY
$ EXIT