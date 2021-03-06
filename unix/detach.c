#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#if	defined(sun) || defined(BSD)
#define USE_BSDSETPGRP
#endif

/*
 * Detach a daemon process from whoever/whatever started it.
 * Mostly lifted from an article in the July/August 1987 ;login:,
 * by Dave Lennert (hplabs!hpda!davel). Blame bugs on me.
 */
/*
 |  [mea@funic.funet.fi]  Lifted this from Rayan Zachariassens
 |			  ZMailer support library.  Handy.
 */


void
detach()
{
	int fd;			/* file descriptor */

	/*
	 * If launched by init (process 1), there's no need to detach.
	 *
	 * Note: this test is unreliable due to an unavoidable race
	 * condition if the process is orphaned.
	 */
	if (getppid() == 1)
		goto out;
	/* Ignore terminal stop signals */
#ifdef	SIGTTOU
	(void) signal(SIGTTOU, SIG_IGN);
#endif	/* SIGTTOU */
#ifdef	SIGTTIN
	(void) signal(SIGTTIN, SIG_IGN);
#endif	/* SIGTTIN */
#ifdef	SIGTSTP
	(void) signal(SIGTSTP, SIG_IGN);
#endif	/* SIGTSTP */
	/*
	 * Allow parent shell to continue.
	 * Ensure the process is not a process group leader.
	 */
	if (fork() != 0)
		exit(0);	/* parent */
	/* child */
	/*
	 * Disassociate from controlling terminal and process group.
	 *
	 * Ensure the process can't reacquire a new controlling terminal.
	 * This is done differently on BSD vs. AT&T:
	 *
	 *	BSD won't assign a new controlling terminal
	 *	because process group is non-zero.
	 *
	 *	AT&T won't assign a new controlling terminal
	 *	because process is not a process group leader.
	 *	(Must not do a subsequent setpgrp()!)
	 */
#ifdef	USE_BSDSETPGRP
	(void) setpgrp(0, getpid());	/* change process group */
	if ((fd = open("/dev/tty", O_RDWR, 0)) >= 0) {
		(void) ioctl(fd, TIOCNOTTY, 0);	/* lose controlling terminal */
		(void) close(fd);
	}
#else	/* !USE_BSDSETPGRP */
	/* lose controlling terminal and change process group */
	(void) setpgrp();
	(void) signal(SIGHUP, SIG_IGN);	/* immunge from pgrp leader death */
	if (fork() != 0)		/* become non-pgrp-leader */
		exit(0);	/* first child */
	/* second child */
#endif	/* USE_BSDSETPGRP */

out:
	(void) close(0);
	(void) close(1);
	(void) close(2);
	(void) umask(022); /* clear any inherited file mode creation mask */

	/* Clean out our environment from personal contamination */
/*	cleanenv(); */

#ifdef	USE_RLIMIT
	/* In case this place runs with cpu limits, remove them */
	{	struct rlimit rl;
		rl.rlim_cur = RLIM_INFINITY;
		rl.rlim_max = RLIM_INFINITY;
		(void) setrlimit(RLIMIT_CPU, &rl);
	}
#endif	/* USE_RLIMIT */
	return;
}


#ifdef	USE_NOFILE
#ifndef	NOFILE
#define	NOFILE 20
#endif	/* NOFILE */

int
getdtablesize()
{
	return NOFILE;
}

#endif	/* USE_NOFILE */
