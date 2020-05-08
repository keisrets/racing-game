struct thread_args
{
    int c_tcp_sockfd;
    int udp_sockfd;
};

// server
void prepare_datagram(char buffer[], char *id1, int x1, int y1, float a1, char *id2, int x2, int y2, float a2);

// client
void unpack_initial_coordinates(char *s_msg, char p_id[], int *p1_x, int *p1_y, float *p1_angle, int *p2_x, int *p2_y, float *p2_angle);
void unpack_datagram(char *s_msg, int *p1_x, int *p1_y, float *p1_a, int *p2_x, int *p2_y, float *p2_a);