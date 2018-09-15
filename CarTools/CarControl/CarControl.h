
#ifndef _CarControl_H_
#define _CarControl_H_

struct _CarControl {
	void (*Forward)(void);
	void (*Backward)(void);
	void (*Left)(void);
	void (*Right)(void);
	void (*Stop)(void);
	void (*ForLeft)(void);
	void (*BackLeft)(void);
	void (*ForRight)(void);
	void (*BackRight)(void);
};

const struct _CarControl *GetCarController(void);

#endif
