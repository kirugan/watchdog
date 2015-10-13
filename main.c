#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <string.h>

#define BUF_LEN 1111

int main(int argc, char** argv){
	int fd = inotify_init();
	if(fd < 0){
		perror("inotify");
		exit(0);
	}

	int wd = inotify_add_watch(fd, "./tmp", IN_CREATE | IN_DELETE);
	if(wd < 0){
		perror("watch");
		exit(0);
	}

	char buf[BUF_LEN];
	char act[20];
	ssize_t len;

	while(1){
		int i = 0;
		len = read(fd, buf, BUF_LEN);

		while(i < len){
			struct inotify_event *event = (struct inotify_event*) &buf[i];
			if(event->len){
				if(event->mask & IN_CREATE){
					strcpy(act, "created");
				} else if(event->mask & IN_DELETE){
					strcpy(act, "deleted");
				} else {
					strcpy(act, "unknown");
				}

				printf("[%s] '%s'\n", act, event->name);
			}

			i += sizeof(struct inotify_event) + event->len;
		}
	}
}