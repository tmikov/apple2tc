new
clear
list

10 REM Start
15 trace
20 goto 30
30 on 1 goto 40
35 print "This should not execute"
40 print "Got to 40"
45 notrace
50 if 2 > 1 then print "2 > 1 ";: print "Yes, really"
60 if 2 < 1 goto 10
70 for i = 1 to 30 step 5: print i: next i
80 gosub 1000
90 on 1 gosub 1000
100 gosub 110
110 pop
120 onerr goto 2000
130 poke 768, 96: print peek(768): call 54162
140 print fre(0)
150 lomem:5000 : himem:10000
160 gr: color=3: plot 1,1: color=4: vlin 0,10 at 3: color=5: hlin 0,10 at 5: print scrn(0,5)
170 hgr: hgr2
180 hcolor=5: hplot 0,0 to 100,100: hplot 30,10
190 rot=0:scale=1:draw 0 at 0,0: xdraw 0 at 0,0
200 text: home
210 input "x=";x
220 print "press a char";: get A$
230 data 100,200
240 read a,b
250 restore
260 print PDL(0)
270 ?22/7
280 htab 1: vtab 1: INVERSE: print "HELLO": FLASH: print " there": NORMAL
290 resume
1000 print "Yeah, a subroutine"
1010 return
2000 print "Error"
2020 end

run
3
a

call -151
L
E003G

