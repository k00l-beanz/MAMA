/// The maximum size the stack can be. May change
#define MAXIMUM_STACK_SIZE 1024

/**********************/
/***** Structures *****/
/**********************/

/// Types of process classes. Can be either application process or system process.
typedef enum {
	/// System Process
	SYS_PROCESS,
	
	/// Application Process
	APPLICATION
} pc_t;

/// Type of Queue Ordering
typedef enum {
	/// Priority Queue (Ready)
	READY,

	/// FIFO Queue (Blocked)
	BLOCKED
} pcb_queue_order_t;

/// Types of process states.
typedef enum {
	/// Ready State
	READY,
	
	/// Running State
	RUNNING,
	
	/// Blocked State
	BLOCKED,
	
	/// Suspended State
	SUSPENDED,
	
	/// Not Suspended State
	N_SUSPENDED
} p_state_t;

/// Process Control Block Structure
typedef struct {
	/// PCB Name
	char pcb_name[32]; 			// Can change size in the future
	
	/// Process Class
	pc_t pcb_process_class;
	
	/// Priority of PCB
	int pcb_priority; 				// Haven't decided wheter 0=lower priority or 9=lower priority. TODO
	
	/// State of the PCB
	p_state_t pcb_process_state;

	// NOTE: Every character in the stack should be initialized to NULL
	// TODO: Determine what the stack size should be
	/// Top of the Stack. Set equal to the stack base + size of the stack
	unsigned char * pcb_stack_top; 
	
	/// Beginning of the Stack
	unsigned char * pcb_stack_bottom;
} pcb_t;

/// "Master" controller of the PCB queue
typedef struct {
	/// Number of PCB's currently in the queue
	int pcbq_count; 	

	/// Head of the PCB queue
	pcb_node_t *pcbq_head; 

	/// Tail of the PCB queue
	pcb_node_t *pcbq_tail; 

	/// Queue order of the Master controller
	pcb_queue_order_t queue_order;
} pcb_queue_t;

/// Individual PCB nodes. Each PCB is associated with one node.
typedef struct {
	/// Pointer to the Next PCB
	struct pcb_node_t *pcbn_next_pcb;

	/// Pointer to the Previous PCB
	struct pcb_node_t *pcbn_prev_pcb;

	/// Pointer to PCB
	pcb_t *pcb;
} pcb_node_t;



/****************************/
/***** Function Headers *****/
/****************************/

/**
 * Allocate memory for a new PCB
 * 
 * Allocates memory for a new PCB in 
 * the stack and performs actions to
 * initialize PCB
 * 
 * @return Pointer to newly created PCB, NULL otherwise 
*/
pcb_t * allocatePCB();

/**
 * Free's memory associated with PCB
 * 
 * Free's the memory associated with the PCB
 * such as the stack and the PCB itself
 * 
 * @param freed_pcb Pointer to the PCB being freed
 * @return Returns 1 upon success, 0 upon error
*/
int freePCB(pcb_t * freed_pcb);

/**
 * Creates a PCB
 * 
 * Allocates and fill memory associated with the PCB being 
 * created. This is accomplished by calling allocatePCB() to
 * initialize the memory and the fills the data with the
 * parameters.
 * 
 * @param name Name of the PCB
 * @param process_class Type of process being created
 * @param priority The priority of the PCB being created
 * 
 * @return Returns pointer to PCB upon success, NULL otherwise 
*/
pcb_t * setupPCB(char * name, pc_t process_class, int priority);

/**
 * Searches for PCB
 * 
 * Given a PCB name, will search all queues for a
 * process.
 * 
 * @param name Name of the PCB being searched
 * 
 * @return Returns pointer to PCB upon success, NULL if PCB was not found
*/
pcb_t * findPCB(char * name);

/**
 * Insert PCB into queue
 * 
 * Inserts a PCB into the appropriate queueu
 * 
 * @param pcb Pointer to the PCB being inserted
*/
void insertPCB(pcb_t * pcb);

/**
 * Removes PCB from Queue
 * 
 * Removes specified PCB from queue
 * it is stored in.
 * 
 * @param pcb Pointer to the PCB being removed
 * 
 * @return Returns 1 upon success, 0 upon error
*/
int removePCB(pcb_t * pcb);