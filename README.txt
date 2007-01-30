tcpmuxclient	- слушает один tcp-порт, и все его коннекты пихает в один поток и скармилвает пайпу
tcpmuxserver	- слушает один поток, и для каждого уникального клиента в нем делает отдельный tcp-connect по одному направлению

udpmuxclient	- ? а имеет ли это смысл? оно ж и так датаграммами, без всяких коннектов-сессий...
udpmuxserver	- ?... разве что ради solid сжатия

overtcp		- делает один коннект на фиксированный host:port, stdin направляет в него, а что читает и него - пишет на stdout.
overudp		- аналогично, только данные пересылаются udp-датаграммами; контроль доставки должен быть, и явно самодельный.
overirc		- запускает кучку ботов в ирц, и сам придумывает как доставлять поток данных.
overicq		-

Пример конструкции:

1). TCP multiplexor до удаленного proxy-сервера с сжатием трафика в gzip (без затравки словаря):

	client: tcpmuxclient localhost 1234 "gzip -9 | tcpclient    A.B.C.D 2345 | gzip -d"
	server: tcpserver      A.D.C.D 2345 "gzip -d | tcpmuxserver PROXYIP 8080 | gzip -9"



2). TCP multiplexor поверх irc (gzipped):

	client: tcpmuxclient localhost 1234
			"gzip -9 |
			over-irc-client
				irc.kraslan.ru:6667:name1:nameA
				irc.kraslan.ru:6667:name2:nameB
				irc.kraslan.ru:6667:name3:nameC
				irc.other.ru:6667:name1:nameA
			| gzip -d"
	server: over-irc-server
			"gzip -d | tcpmuxserver PROXYIP 8080 | gzip -9"
				irc.crownet.ru:6667:nameA:name1
				irc.crownet.ru:6667:nameB:name2
				irc.kraslan.ru:6667:nameC:name3
				irc.other.ru:6667:nameA:name1
	Здесь предаолагается что сервера irc.kraslan.ru & irc.crownet.ru слинкованы, а irc.other.ru - сам по себе.
	Первое имя - какой ник взять себе. Второе - с каким ником общаться.
