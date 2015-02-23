::To be launched within the SDK shell!!
@echo off
data2mem -bm .\hw_hw_platform_0\design_2_wrapper_bd.bmm -bt .\hw_hw_platform_0\design_2_wrapper.bit ^
-bd .\decoder_merger\Release\decoder_merger.elf tag design_1_i_MbCluster0_mb02_64Kb_microblaze_0 ^
-bd .\decoder_motion_add\Release\decoder_motion_add.elf tag design_1_i_MbCluster0_mb03_64Kb_microblaze_0 ^
-bd .\decoder_motion_framebuf\Release\decoder_motion_framebuf.elf tag design_1_i_MbCluster0_mb01_128Kb_microblaze_0 ^
-bd .\decoder_motion_interpolation\Release\decoder_motion_interpolation.elf tag design_1_i_MbCluster0_mb04_64Kb_microblaze_0 ^
-bd .\decoder_parser_blkexp\Release\decoder_parser_blkexp.elf tag design_1_i_MbCluster0_mb05_64Kb_microblaze_0 ^
-bd .\decoder_parser_mvrecon\Release\decoder_parser_mvrecon.elf tag design_1_i_MbCluster0_mb06_64Kb_microblaze_0 ^
-bd .\decoder_parser_mvseq\Release\decoder_parser_mvseq.elf tag design_1_i_MbCluster0_mb07_64Kb_microblaze_0 ^
-bd .\decoder_parser_parseheaders\Release\decoder_parser_parseheaders.elf tag design_1_i_MbCluster0_mb00_256Kb_microblaze_0 ^
-bd .\decoder_texture_DCReconstruction_addressing\Release\decoder_texture_DCReconstruction_addressing.elf tag design_1_i_MbCluster1_mb01_128Kb_microblaze_0 ^
-bd .\decoder_texture_DCReconstruction_invpred\Release\decoder_texture_DCReconstruction_invpred.elf tag design_1_i_MbCluster1_mb02_64Kb_microblaze_0 ^
-bd .\decoder_texture_DCsplit\Release\decoder_texture_DCsplit.elf tag design_1_i_MbCluster1_mb03_64Kb_microblaze_0 ^
-bd .\decoder_texture_IAP\Release\decoder_texture_IAP.elf tag design_1_i_MbCluster1_mb04_64Kb_microblaze_0 ^
-bd .\decoder_texture_idct2d\Release\decoder_texture_Idct2d.elf tag design_1_i_MbCluster1_mb05_64Kb_microblaze_0 ^
-bd .\decoder_texture_IQ\Release\decoder_texture_IQ.elf tag design_1_i_MbCluster1_mb06_64Kb_microblaze_0 ^
-bd .\decoder_texture_IS\Release\decoder_texture_IS.elf tag design_1_i_MbCluster1_mb07_64Kb_microblaze_0 ^
-o b .\hw_platform_1\download_release.bit
data2mem -bm .\hw_platform_1\design_1_wrapper_bd.bmm -bt .\hw_hw_platform_0\design_1_wrapper.bit ^
-bd .\decoder_merger\Debug\decoder_merger.elf tag design_1_i_MbCluster0_mb02_64Kb_microblaze_0 ^
-bd .\decoder_motion_add\Debug\decoder_motion_add.elf tag design_1_i_MbCluster0_mb03_64Kb_microblaze_0 ^
-bd .\decoder_motion_framebuf\Debug\decoder_motion_framebuf.elf tag design_1_i_MbCluster0_mb01_128Kb_microblaze_0 ^
-bd .\decoder_motion_interpolation\Debug\decoder_motion_interpolation.elf tag design_1_i_MbCluster0_mb04_64Kb_microblaze_0 ^
-bd .\decoder_parser_blkexp\Debug\decoder_parser_blkexp.elf tag design_1_i_MbCluster0_mb05_64Kb_microblaze_0 ^
-bd .\decoder_parser_mvrecon\Debug\decoder_parser_mvrecon.elf tag design_1_i_MbCluster0_mb06_64Kb_microblaze_0 ^
-bd .\decoder_parser_mvseq\Debug\decoder_parser_mvseq.elf tag design_1_i_MbCluster0_mb07_64Kb_microblaze_0 ^
-bd .\decoder_parser_parseheaders\Debug\decoder_parser_parseheaders.elf tag design_1_i_MbCluster0_mb00_256Kb_microblaze_0 ^
-bd .\decoder_texture_DCReconstruction_addressing\Debug\decoder_texture_DCReconstruction_addressing.elf tag design_1_i_MbCluster1_mb01_128Kb_microblaze_0 ^
-bd .\decoder_texture_DCReconstruction_invpred\Debug\decoder_texture_DCReconstruction_invpred.elf tag design_1_i_MbCluster1_mb02_64Kb_microblaze_0 ^
-bd .\decoder_texture_DCsplit\Debug\decoder_texture_DCsplit.elf tag design_1_i_MbCluster1_mb03_64Kb_microblaze_0 ^
-bd .\decoder_texture_IAP\Debug\decoder_texture_IAP.elf tag design_1_i_MbCluster1_mb04_64Kb_microblaze_0 ^
-bd .\decoder_texture_idct2d\Debug\decoder_texture_Idct2d.elf tag design_1_i_MbCluster1_mb05_64Kb_microblaze_0 ^
-bd .\decoder_texture_IQ\Debug\decoder_texture_IQ.elf tag design_1_i_MbCluster1_mb06_64Kb_microblaze_0 ^
-bd .\decoder_texture_IS\Debug\decoder_texture_IS.elf tag design_1_i_MbCluster1_mb07_64Kb_microblaze_0 ^
-o b .\hw_platform_1\download_debug.bit