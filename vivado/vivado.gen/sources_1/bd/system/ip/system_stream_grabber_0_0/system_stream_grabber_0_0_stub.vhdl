-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2022.1 (lin64) Build 3526262 Mon Apr 18 15:47:01 MDT 2022
-- Date        : Sun Nov  9 23:44:05 2025
-- Host        : edalinux20 running 64-bit Rocky Linux release 8.10 (Green Obsidian)
-- Command     : write_vhdl -force -mode synth_stub
--               /home/christopherlai/lab3a/vivado.gen/sources_1/bd/system/ip/system_stream_grabber_0_0/system_stream_grabber_0_0_stub.vhdl
-- Design      : system_stream_grabber_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7a100tcsg324-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity system_stream_grabber_0_0 is
  Port ( 
    sys_clk : in STD_LOGIC;
    sys_rst : in STD_LOGIC;
    s_axis_stream_tdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axis_stream_tvalid : in STD_LOGIC;
    s_axis_stream_tready : out STD_LOGIC;
    s_axi_cpu_awaddr : in STD_LOGIC_VECTOR ( 11 downto 0 );
    s_axi_cpu_awvalid : in STD_LOGIC;
    s_axi_cpu_awready : out STD_LOGIC;
    s_axi_cpu_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_cpu_wvalid : in STD_LOGIC;
    s_axi_cpu_wready : out STD_LOGIC;
    s_axi_cpu_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_cpu_bvalid : out STD_LOGIC;
    s_axi_cpu_bready : in STD_LOGIC;
    s_axi_cpu_araddr : in STD_LOGIC_VECTOR ( 11 downto 0 );
    s_axi_cpu_arvalid : in STD_LOGIC;
    s_axi_cpu_arready : out STD_LOGIC;
    s_axi_cpu_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_cpu_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_cpu_rvalid : out STD_LOGIC;
    s_axi_cpu_rready : in STD_LOGIC
  );

end system_stream_grabber_0_0;

architecture stub of system_stream_grabber_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "sys_clk,sys_rst,s_axis_stream_tdata[31:0],s_axis_stream_tvalid,s_axis_stream_tready,s_axi_cpu_awaddr[11:0],s_axi_cpu_awvalid,s_axi_cpu_awready,s_axi_cpu_wdata[31:0],s_axi_cpu_wvalid,s_axi_cpu_wready,s_axi_cpu_bresp[1:0],s_axi_cpu_bvalid,s_axi_cpu_bready,s_axi_cpu_araddr[11:0],s_axi_cpu_arvalid,s_axi_cpu_arready,s_axi_cpu_rdata[31:0],s_axi_cpu_rresp[1:0],s_axi_cpu_rvalid,s_axi_cpu_rready";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "stream_grabber,Vivado 2022.1";
begin
end;
