#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
    int i, clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    if (vbdOpen() != 1) return -1;
    vbdHeader("Lab1: Counter");
    vbdSetMode(1);

    // initialise simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;
    top->ld = 0;
    // top->v = 0;
    
    // run simulation for many clock cycles
    for (i = 0; i < 600; i++) {
        
        // dump vars into vcd file and toggle clock
        for (clk = 0; clk < 2; clk++) {
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }
        // send count value to vbd
        // vbdHex(4, (int(top->count) >> 16) & 0x0F);
        // vbdHex(3, (int(top->count) >> 8) & 0x0F);
        // vbdHex(2, (int(top->count) >> 4) & 0x0F);
        // vbdHex(1, int(top->count) & 0x0F);
        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i+1);

        top->rst = (i < 2);
        // top->v = vbdValue();
        top->en = (i > 4);
        top->ld = vbdFlag();
        if (Verilated::gotFinish()) exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}