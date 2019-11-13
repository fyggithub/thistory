import sys
reload(sys)
sys.setdefaultencoding('utf-8')

import os
import math
import numpy as np
import pandas as pd
import ConfigParser as cfg

#################################################################
##
##
#################################################################
def CheckField(fields, df):
    for f in fields:
        if f not in df:
            print 'Not Found Field', f
            return False
    return True


#################################################################
##
##
#################################################################
def ToString(d):
    if type(d) == float:
        return ''
    else:
        return d

def ToInt(d):
    if type(d) == np.float64:
        if not math.isnan(d):
            return int(d)
    elif type(d) == np.int64:
        return d
    # print type(d)
    return 0
#################################################################
##
##
#################################################################

def GetConfigNumber(obj):
    n = 0
    for item in obj:
        if item['IS_CONFIG'] == 'YES':
            n += 1
    return n

def ProcessDataframe(df):
    obj = []
    if not CheckField(['ID', 'NAME', 'COMMENT', 'IS_CONFIG',
                   'DEFAULT', 'MIN', 'MAX'],
                   df):
        print 'The Excel Missed Some Field!!!'
    else:
        for i in xrange(len(df)):
            item = {}
            item['ID'] = ToString(df.iloc[i]['ID'])
            if item['ID'] == '':
                continue
            item['NAME']      = ToString(df.iloc[i]['NAME'])
            item['COMMENT']   = ToString(df.iloc[i]['COMMENT'])
            item['IS_CONFIG'] = ToString(df.iloc[i]['IS_CONFIG'])
            item['DEFAULT']   = ToInt(df.iloc[i]['DEFAULT'])
            item['MIN']       = ToInt(df.iloc[i]['MIN'])
            item['MAX']       = ToInt(df.iloc[i]['MAX'])

            if 'SHORT' in df:
                item['SHORT'] = ToString(df.iloc[i]['SHORT'])
            else:
                item['SHORT'] = ''

            if 'POSITION' in df:
                item['POSITION'] = ToString(df.iloc[i]['POSITION'])
            else:
                item['POSITION'] = ''

            obj.append(item)
    return obj
#################################################################
##
##
#################################################################
def WriteFileHeader(fileHdl):
    fileHdl.write('/*********************************************************\n')
    fileHdl.write(' * Shenzhen Thistroy Bio. Ltd. Co. copyright\n')
    fileHdl.write(' * -- Created by software, DO NOT Modify\n')
    fileHdl.write(' ********************************************************/\n\n')

def WriteIfdef(fileHdl, baseName):
    fileHdl.write('#ifndef _%s_H_\n'%baseName.upper())
    fileHdl.write('#define _%s_H_\n\n'%baseName.upper())

def WriteEndif(fileHdl):
    fileHdl.write('\n#endif\n')

def WriteInclude(fileHdl, baseName):
    fileHdl.write('#include "%s.h"\n\n'%baseName)

#################################################################
##
##
#################################################################
def WriteConstDefine(fileHdl, obj):
    for item in obj:
        if item['IS_CONFIG'] != 'YES':
            line = '#define CONST_%-42s %8d // %s\n'%(
                item['ID'], item['DEFAULT'], item['NAME']
            )
            fileHdl.write(line)

def WriteEnumDefine(fileHdl, obj):
    fileHdl.write('\nenum{\n')
    locStr = '= CFG_USER_BEGIN'
    for item in obj:
        if item['IS_CONFIG'] == 'YES':
            line = '\tCFG_%-40s%-16s, // %s\n'%(
                item['ID'], locStr, item['NAME']
            )
            fileHdl.write(line)
            locStr = ''
    fileHdl.write('\tCFG_MAX\n')
    fileHdl.write('};\n')
#################################################################
##
##
#################################################################
def WriteDefault(fileHdl, obj):
    fileHdl.write(
        'CONST ConfigDefault configDefault[CFG_MAX-CFG_USER_BEGIN] = {\n')
    for item in obj:
        if item['IS_CONFIG'] == 'YES':
            line = '\t{%12s, %12s, %12s},// %s\n'%(
                item['DEFAULT'], item['MIN'], item['MAX'], item['NAME']
            )
            fileHdl.write(line)
    fileHdl.write('};\n')
#################################################################
##
##
#################################################################
def MakeHeader(fileHdl, obj, baseName):
    WriteFileHeader(fileHdl)
    WriteIfdef(fileHdl, baseName)
    fileHdl.write('#include "Datatypes.h"\n')
    fileHdl.write('#include "Config.h"\n\n')
    WriteConstDefine(fileHdl, obj)
    WriteEnumDefine(fileHdl, obj)
    fileHdl.write(
        '\nextern CONST ConfigDefault configDefault[CFG_MAX-CFG_USER_BEGIN];\n'
    )
    WriteEndif(fileHdl)

#################################################################
##
##
#################################################################
def MakeSource(fileHdl, obj, baseName):
    WriteFileHeader(fileHdl)
    fileHdl.write('#include "Config.h"\n')
    WriteInclude(fileHdl, baseName)
    WriteDefault(fileHdl, obj)


#################################################################
##
##
#################################################################

def MakeIniConfig(fileHdl, obj):
    config = cfg.ConfigParser()
    idx = 16
    for item in obj:
        if item['IS_CONFIG'] == 'YES':
            sec = item['ID']
            config.add_section(sec)
            config.set(sec, "id", idx)
            config.set(sec, "name", item['NAME'])
            # config.set(sec, "comment", item['COMMENT'])
            config.set(sec, "min", item['MIN'])
            config.set(sec, "max", item['MAX'])
            config.set(sec, "short", item['SHORT'])
            if item['POSITION'] == 'YES':
                config.set(sec, "position", 1)
            else:
                config.set(sec, "position", 0)
            idx+=1
    config.write(fileHdl)

#################################################################
##
##
#################################################################
def MakeOneConfig(xlsxFilename):
    df  = pd.read_excel(xlsxFilename)
    obj = ProcessDataframe(df)
    baseName = os.path.splitext(xlsxFilename)[0]

    if GetConfigNumber(obj) == 0:
        item = {'ID':'CFG_UNUSED',
                'NAME':'UNUSED',
                'COMMENT':'',
                'IS_CONFIG':'YES',
                'DEFAULT':0,
                'MIN':0,
                'MAX':0}
        obj.append(item)
    else:
        iniName = baseName+'.ini'
        with open(iniName, 'w') as fileHdl:
            MakeIniConfig(fileHdl, obj)

    headerName = baseName+'.h'
    with open(headerName, 'w') as fileHdl:
        MakeHeader(fileHdl, obj, baseName)
    sourceName = baseName+'.c'
    with open(sourceName, 'w') as fileHdl:
        MakeSource(fileHdl, obj, baseName)


#################################################################
##
##
#################################################################


def test_main():
    filename = 'test.xlsx'
    MakeOneConfig(filename)
    pass


def main():
    path = './'
    ext = '.xlsx'
    for filename in os.listdir(path):
        if filename.endswith(ext):
            print 'Processing ' + filename + '...'
            MakeOneConfig(filename)
    pass


#################################################################
##
##
#################################################################
if __name__ == '__main__':
    if False:
        test_main()
    else:
        main()
    pass