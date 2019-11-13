@echo off
echo Rename Input Files...
copy ".\input\01试剂针.xlsx"    ".\ConfigReage.xlsx"
copy ".\input\02样本针.xlsx"    ".\ConfigSample.xlsx"
copy ".\input\03抓手.xlsx"      ".\ConfigGrab.xlsx"
copy ".\input\04自动装杯.xlsx"   ".\ConfigLoad.xlsx"
copy ".\input\05测量中枢板.xlsx" ".\ConfigMeas.xlsx"
copy ".\input\06孵育加热.xlsx"   ".\ConfigHeat.xlsx"
copy ".\input\07试剂制冷.xlsx"   ".\ConfigCold.xlsx"
copy ".\input\08液面检测.xlsx"   ".\ConfigLiquid.xlsx"

del *.h
del *.c
del ".\output\*.h"
del ".\output\*.c"

del *.ini
del ".\ini\*.ini"

python MakeConfig.py

copy *.h ".\output\"
copy *.c ".\output\"
copy *.ini ".\ini\"

copy ".\output\ConfigReage.*" "..\..\Source\Boards\Rege\"
copy ".\output\ConfigSample.*" "..\..\Source\Boards\Sample\"
copy ".\output\ConfigGrab.*" "..\..\Source\Boards\Grab\"
copy ".\output\ConfigLoad.*" "..\..\Source\Boards\Load\"
copy ".\output\ConfigMeas.*" "..\..\Source\Boards\Pd&MagHub\"
copy ".\output\ConfigHeat.*" "..\..\Source\Boards\Heat\"
copy ".\output\ConfigCold.*" "..\..\Source\Boards\Cool\"
copy ".\output\ConfigLiquid.*" "..\..\Source\Boards\Liquid\"

echo Delete Temp. Files...
del *.xlsx
del *.h
del *.c
del *.ini

echo -
echo =================================
echo Have Done! Close Me!
echo =================================
echo -
pause