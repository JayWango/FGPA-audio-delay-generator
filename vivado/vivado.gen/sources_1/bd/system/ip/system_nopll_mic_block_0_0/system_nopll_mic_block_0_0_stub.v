// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2022.1 (lin64) Build 3526262 Mon Apr 18 15:47:01 MDT 2022
// Date        : Sun Nov  9 23:44:07 2025
// Host        : edalinux20 running 64-bit Rocky Linux release 8.10 (Green Obsidian)
// Command     : write_verilog -force -mode synth_stub
//               /home/christopherlai/lab3a/vivado.gen/sources_1/bd/system/ip/system_nopll_mic_block_0_0/system_nopll_mic_block_0_0_stub.v
// Design      : system_nopll_mic_block_0_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "nopll_mic_block,Vivado 2022.1" *)
module system_nopll_mic_block_0_0(sys_clk, sys_rst, mic_clk, mic_lr_sel, 
  mic_data_in, m_axis_tdata, m_axis_tvalid, m_axis_tready)
/* synthesis syn_black_box black_box_pad_pin="sys_clk,sys_rst,mic_clk,mic_lr_sel,mic_data_in,m_axis_tdata[31:0],m_axis_tvalid,m_axis_tready" */;
  input sys_clk;
  input sys_rst;
  output mic_clk;
  output mic_lr_sel;
  input mic_data_in;
  output [31:0]m_axis_tdata;
  output m_axis_tvalid;
  input m_axis_tready;
endmodule
