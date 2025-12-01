# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: /home/jasonwang/Desktop/ece253/grad_proj/vitis/grad_proj_application_system/_ide/scripts/debugger_grad_proj_application-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source /home/jasonwang/Desktop/ece253/grad_proj/vitis/grad_proj_application_system/_ide/scripts/debugger_grad_proj_application-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -filter {jtag_cable_name =~ "Digilent Nexys A7 -100T 210292BCFA4CA" && level==0 && jtag_device_ctx=="jsn-Nexys A7 -100T-210292BCFA4CA-13631093-0"}
fpga -file /home/jasonwang/Desktop/ece253/grad_proj/vitis/grad_proj_application/_ide/bitstream/grad_proj_hw_platform.bit
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
loadhw -hw /home/jasonwang/Desktop/ece253/grad_proj/vitis/grad_proj_platform/export/grad_proj_platform/hw/grad_proj_hw_platform.xsa -regs
configparams mdm-detect-bscan-mask 2
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
rst -system
after 3000
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
dow /home/jasonwang/Desktop/ece253/grad_proj/vitis/grad_proj_application/Debug/grad_proj_application.elf
bpadd -addr &main
