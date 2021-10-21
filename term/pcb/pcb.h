#ifndef PCB_H
#define PCB_H

/// The maximum size the stack can be. May change
#define MAX_STACK_SIZE 1024

/// Maximum priority a PCB can be given
#define MAX_PRIORITY 9
/// Minimum priority a PCB can be given
#define MIN_PRIORITY 0

/// Maximum name size that can be given to a pcb
#define MAX_NAME_SIZE 32

/********************************************/
/**************** Structures ****************/
/********************************************/

/// Type of Queue Ordering
typedef enum {
    /// Priority Queue (Ready)
    PRIORITY,

    /// FIFO Queue (Blocked)
    FIFO
} pcb_queue_order_t;

/// Types of process states.
typedef enum {
    /// Running State
    RUNNING,

    /// Ready State
    READY,
    
    /// Blocked State
    BLOCKED,

    /// Suspended Ready State
    SUSPENDED_READY,

    /// Suspended Blocked State
    SUSPENDED_BLOCKED
} p_state_t;

/// Process Control Block Structure
typedef struct {
    /// PCB Name
    char pcb_name[32];          // Can change size in the future
    
    /// Process Class
    int pcb_process_class;     // I've decided that process class will be an int. SYS_PROCESS = 0, APPLICATION = 1
    
    /// Priority of PCB
    int pcb_priority;
    
    /// State of the PCB
    p_state_t pcb_process_state;

    /// Top of the Stack. Set equal to the stack base + size of the stack
    unsigned char * pcb_stack_top; 
    
    /// Beginning of the Stack
    unsigned char * pcb_stack_bottom;
} pcb_t;

/// Individual PCB nodes. Each PCB is associated with one node.
typedef struct pcb_node_t {
    /// Pointer to the Next PCB
    struct pcb_node_t *pcbn_next_pcb;

    /// Pointer to the Previous PCB
    struct pcb_node_t *pcbn_prev_pcb;

    /// Pointer to PCB
    pcb_t *pcb;
} pcb_node_t;

/// "Master" controller of the PCB queue
typedef struct pcb_queue {
    /// Number of PCB's currently in the queue
    int pcbq_count;     

    /// Head of the PCB queue
    pcb_node_t *pcbq_head; 

    /// Tail of the PCB queue
    pcb_node_t *pcbq_tail; 

    /// Queue order of the Master controller
    pcb_queue_order_t queue_order;
} pcb_queue_t;



/********************************************/
/************ Function Headers **************/
/********************************************/

/**
 * Initialize PCB Queue
 * 
 * Initialize the PCB queue's by assigning values for the two
 * queues that exist. This method is called upon startup in the 
 * commhand 
*/
void initPCB();

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
pcb_t * setupPCB(char * name, int process_class, int priority);

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
 * Inserts a PCB into the appropriate queue
 * 
 * @param pcb Pointer to the PCB being inserted
 *
 * @return 0 on success, 1 on error
*/
int insertPCB(pcb_t * pcb);

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


/**
 * Create a PCB
 * 
 * Creates a new, unique PCB in memory.  
 * 
 * @param name Give name of the PCB
 * @param process_class The type of process class that will be used
 * @param priority Priority of the PCB
 * 
 * @return Returns 0 upon success, 1 upon error
*/
int createPCB(char * user_input);

/**
 * Delete PCB
 * 
 * Will remove a PCB from the appropriate queue 
 * and free all associated memory. Will find the PCB 
 * in the queue, unlink it and free it.
 * 
 * @param name Name of the PCB to delete
 * 
 * @return Return 0 upon success, 1 upon failure
 * 
*/
int deletePCB(char * name);

/**
 * Set PCB state to be blocked
 * 
 * Find the PCB name in queue and sets
 * its state to blocked and reinserts it into
 * the appropriate queue.
 * 
 * @param name Name of PCB to block
*/
int blockPCB(char * name);

/**
 * Set PCB state to unblocked
 * 
 * Sets PCB state into unblocked and
 * reinserts it into the appropriate queue
 * 
 * @param name Name of the PCB to unblock 
 * 
 * @return Returns 0 upon success, 1 upon error
*/
int unblockPCB(char * name);

/**
 * Set PCB state to suspended
 * 
 * Places a PCB state into suspended
 * and reinserts into appropriate queue
 * 
 * @param name Name of PCB to suspend
 * 
 * @return Returns 0 upon success, 1 upon error
*/
int suspendPCB(char * name);

/**
 * Set PCB state to resume
 * 
 * Places a PCB into a not suspended state
 * and reinserts into the appropriate queue
 * 
 * @param name Name of PCB to resume 
 * 
 * @return Returns 0 upon success, 1 upon error
*/
int resumePCB(char * name);

/**
 * Set a new priority to a PCB
 * 
 * Sets a PCB's priority and reinserts the process into
 * the correct place in the correct queue
 * 
 * @param args Name of the PCB and new priority (PCB_NAME.PRIORITY)
 * 
 * @return Returns 0 upon success, 1 upon error 
*/
int setPriority(char * args);

/**
 * Show informatino of PCB
 * 
 * Display information of the PCB. The information
 * that is displayed is: Process Name, Class, State,
 * Suspended Status and Priority 
 * 
 * @param name Name of PCB to have its information displayed
*/
int showPCB(char * name);

/**
 * Show PCBs in ready queue
 * 
 * Display information for each PCB in the ready queue.
 * The information that is displayed is: Process Name, Class,
 * State, Suspended Status, Priority
 * 
 * @param p Empty parameters.
 * 
 * @return 0 upon success, 1 upon failure
*/
int showReady(char * p);

/**
 * Show PCBs in blocked queue
 * 
 * Display information for each PCB in the blocked queue.
 * The information that is displayed is: Process Name, Class,
 * State, Suspended Status, Priority.
 * 
*/
int showBlocked(char * args);

/**
 * Show all PCBs
 * 
 * Display information for each PCB in the ready and blocked queue.
 * The information that is displayed is: Process Name, Class,
 * State, Suspended Status, Priority. 
 * 
 * @params args Empty params
 * 
 * @return Returns 0 upon success, 1 upon error
*/
int showAll(char * args);

/********************************************************/
/********************* R4 Stuff Here ********************/
/********************************************************/

/**
 * Resume all suspended processes.
 * 
 * Iterates through READY queue and sets the
 * state of the each PCB to READY
 * 
 * @param p Empty params
 * 
 * @return Returns 0 upon success, -1 otherwise.
 * 
*/
int resumeAll(char * p);

#endif