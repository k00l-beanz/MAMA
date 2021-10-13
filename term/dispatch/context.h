
/// Context of the currently operating process
typedef struct Context {
	/// Segment registers
	u32int gs, fs, es, ds;

	/// General purpose registers
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;

	// Other special registers
	u32int eip, cs, eflags;
} context;