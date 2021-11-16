#include "1m-block.hpp"

bool debug = false;
sqlite3* db;
sqlite3_stmt* res;

int main(int argc, char **argv)
{
	// add usage
	if(argc < 2){
		printf("syntax : %s <site list file> [-d]\n", argv[0]);
		printf("sample : %s top-1m.csv [-d]\n", argv[0]);
		printf("<site list file> must be .csv file or .db file\n");
		printf("option : -d for debug mode\n");
	}
	// debug mode
	else if (argc == 3)
		if(string(argv[2]) == "-d")
			debug = true;

	string database_name;

	if(string(argv[1]).find(".db")==string::npos){
		// make database
		ifstream fin(argv[1]);
		if(fin.fail()){
			printf("There is no %s file.", argv[1]);
			return 0;
		}
		// database name
		database_name = string(argv[1]) + ".db";
		// open database
		int rc = sqlite3_open(database_name.c_str(), &db);
		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return 1;
		}
		
		if(database_init(fin) == 1) // sql error occured;
		{
			fin.close();
			return 1;
		}
		fin.close();

	}
	else{
		database_name = string(argv[1]);
		// open database
		int rc = sqlite3_open(database_name.c_str(), &db);
		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return 1;
		}
	}
	// for safty exit
	signal(SIGINT, sigint_handler);
	
	struct nfq_handle *h;
	struct nfq_q_handle *qh;
	struct nfnl_handle *nh;
	int fd;
	int rv;
	char buf[4096] __attribute__ ((aligned));

	printf("opening library handle\n");
	h = nfq_open();
	if (!h) {
		fprintf(stderr, "error during nfq_open()\n");
		exit(1);
	}

	printf("unbinding existing nf_queue handler for AF_INET (if any)\n");
	if (nfq_unbind_pf(h, AF_INET) < 0) {
		fprintf(stderr, "error during nfq_unbind_pf()\n");
		exit(1);
	}

	printf("binding nfnetlink_queue as nf_queue handler for AF_INET\n");
	if (nfq_bind_pf(h, AF_INET) < 0) {
		fprintf(stderr, "error during nfq_bind_pf()\n");
		exit(1);
	}

	// I modify cb(callback) function
	printf("binding this socket to queue '0'\n");
	qh = nfq_create_queue(h,  0, &cb, NULL);
	if (!qh) {
		fprintf(stderr, "error during nfq_create_queue()\n");
		exit(1);
	}

	printf("setting copy_packet mode\n");
	if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
		fprintf(stderr, "can't set packet_copy mode\n");
		exit(1);
	}

	fd = nfq_fd(h);

	for (;;) {
		if ((rv = recv(fd, buf, sizeof(buf), 0)) >= 0) {
			printf("pkt received\n");
			nfq_handle_packet(h, buf, rv);
			continue;
		}
		/* if your application is too slow to digest the packets that
		 * are sent from kernel-space, the socket buffer that we use
		 * to enqueue packets may fill up returning ENOBUFS. Depending
		 * on your application, this error may be ignored. nfq_nlmsg_verdict_putPlease, see
		 * the doxygen documentation of this library on how to improve
		 * this situation.
		 */
		if (rv < 0 && errno == ENOBUFS) {
			printf("losing packets!\n");
			continue;
		}
		perror("recv failed");
		break;
	}

	printf("unbinding from queue 0\n");
	nfq_destroy_queue(qh);

#ifdef INSANE
	/* normally, applications SHOULD NOT issue this command, since
	 * it detaches other programs/sockets from AF_INET, too ! */
	printf("unbinding from AF_INET\n");
	nfq_unbind_pf(h, AF_INET);
#endif

	printf("closing library handle\n");
	nfq_close(h);

	exit(0);
}
