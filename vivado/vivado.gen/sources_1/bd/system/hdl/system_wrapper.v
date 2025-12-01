//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2022.1 (lin64) Build 3526262 Mon Apr 18 15:47:01 MDT 2022
//Date        : Thu Nov 27 13:32:31 2025
//Host        : edalinux05 running 64-bit Rocky Linux release 8.10 (Green Obsidian)
//Command     : generate_target system_wrapper.bd
//Design      : system_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module system_wrapper
   (DDR2_addr,
    DDR2_ba,
    DDR2_cas_n,
    DDR2_ck_n,
    DDR2_ck_p,
    DDR2_cke,
    DDR2_cs_n,
    DDR2_dm,
    DDR2_dq,
    DDR2_dqs_n,
    DDR2_dqs_p,
    DDR2_odt,
    DDR2_ras_n,
    DDR2_we_n,
    JA,
    LED,
    SW,
    an,
    aud_sd,
    audio_pwm_output,
    btn,
    btnCpuReset,
    clock_rtl,
    io0_o,
    mic_clk,
    mic_data_in,
    mic_lr_sel,
    reset_spi,
    rgbleds,
    sck_o,
    seg,
    spi_dc,
    ss_o);
  output [12:0]DDR2_addr;
  output [2:0]DDR2_ba;
  output DDR2_cas_n;
  output [0:0]DDR2_ck_n;
  output [0:0]DDR2_ck_p;
  output [0:0]DDR2_cke;
  output [0:0]DDR2_cs_n;
  output [1:0]DDR2_dm;
  inout [15:0]DDR2_dq;
  inout [1:0]DDR2_dqs_n;
  inout [1:0]DDR2_dqs_p;
  output [0:0]DDR2_odt;
  output DDR2_ras_n;
  output DDR2_we_n;
  input [2:0]JA;
  output [15:0]LED;
  input [15:0]SW;
  output [7:0]an;
  output [0:0]aud_sd;
  output audio_pwm_output;
  input [4:0]btn;
  input btnCpuReset;
  input clock_rtl;
  output io0_o;
  output mic_clk;
  input mic_data_in;
  output mic_lr_sel;
  output reset_spi;
  output [5:0]rgbleds;
  output sck_o;
  output [6:0]seg;
  output [0:0]spi_dc;
  output [0:0]ss_o;

  wire [12:0]DDR2_addr;
  wire [2:0]DDR2_ba;
  wire DDR2_cas_n;
  wire [0:0]DDR2_ck_n;
  wire [0:0]DDR2_ck_p;
  wire [0:0]DDR2_cke;
  wire [0:0]DDR2_cs_n;
  wire [1:0]DDR2_dm;
  wire [15:0]DDR2_dq;
  wire [1:0]DDR2_dqs_n;
  wire [1:0]DDR2_dqs_p;
  wire [0:0]DDR2_odt;
  wire DDR2_ras_n;
  wire DDR2_we_n;
  wire [2:0]JA;
  wire [15:0]LED;
  wire [15:0]SW;
  wire [7:0]an;
  wire [0:0]aud_sd;
  wire audio_pwm_output;
  wire [4:0]btn;
  wire btnCpuReset;
  wire clock_rtl;
  wire io0_o;
  wire mic_clk;
  wire mic_data_in;
  wire mic_lr_sel;
  wire reset_spi;
  wire [5:0]rgbleds;
  wire sck_o;
  wire [6:0]seg;
  wire [0:0]spi_dc;
  wire [0:0]ss_o;

  system system_i
       (.DDR2_addr(DDR2_addr),
        .DDR2_ba(DDR2_ba),
        .DDR2_cas_n(DDR2_cas_n),
        .DDR2_ck_n(DDR2_ck_n),
        .DDR2_ck_p(DDR2_ck_p),
        .DDR2_cke(DDR2_cke),
        .DDR2_cs_n(DDR2_cs_n),
        .DDR2_dm(DDR2_dm),
        .DDR2_dq(DDR2_dq),
        .DDR2_dqs_n(DDR2_dqs_n),
        .DDR2_dqs_p(DDR2_dqs_p),
        .DDR2_odt(DDR2_odt),
        .DDR2_ras_n(DDR2_ras_n),
        .DDR2_we_n(DDR2_we_n),
        .JA(JA),
        .LED(LED),
        .SW(SW),
        .an(an),
        .aud_sd(aud_sd),
        .audio_pwm_output(audio_pwm_output),
        .btn(btn),
        .btnCpuReset(btnCpuReset),
        .clock_rtl(clock_rtl),
        .io0_o(io0_o),
        .mic_clk(mic_clk),
        .mic_data_in(mic_data_in),
        .mic_lr_sel(mic_lr_sel),
        .reset_spi(reset_spi),
        .rgbleds(rgbleds),
        .sck_o(sck_o),
        .seg(seg),
        .spi_dc(spi_dc),
        .ss_o(ss_o));
endmodule
