find . -name "bsp_cluster?_mb4/system.mss -exec awk '$2 == "HW_INSTANCE" && length($4)==1 {sub($4,"ps7_ddr_0",$4)} {print $0}' > temp -exec cp temp ./bsp_cluster?_mb4/system.mss 
