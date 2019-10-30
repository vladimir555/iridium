/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
////@version=3
//strategy("Pivoting Pivot HL", overlay=true, default_qty_type=strategy.cash, default_qty_value=100000, pyramiding=0)
//FromMonth = input(defval = 6, title = "From Month", minval = 1, maxval = 12)
//FromDay   = input(defval = 1, title = "From Day", minval = 1, maxval = 31)
//FromYear  = input(defval = 2019, title = "From Year", minval = 2010)
//ToMonth   = input(defval = 12, title = "To Month", minval = 1, maxval = 12)
//ToDay     = input(defval = 31, title = "To Day", minval = 1, maxval = 31)
//ToYear    = input(defval = 2019, title = "To Year", minval = 2017)

//// === FUNCTION EXAMPLE ===
//start     = timestamp(FromYear, FromMonth, FromDay, 00, 00)  // backtest start window
//finish    = timestamp(ToYear, ToMonth, ToDay, 23, 59)        // backtest finish window
//window()  => time >= start and time <= finish ? true : false // create function "within window of time"


//d = input(3)
//ph = pivothigh(high,d,d)
//pl = pivotlow(low,d,d)

//cc = not na(ph)
//cc2 = not na(pl)

//hp = 0.0
//hp := cc ? ph : hp[1]
//hp2 = 0.0
//hp2 := cc2 ? pl : hp2[1]

//le = false
//le := cc ? true : (le[1] and high > hp ? false : le[1])
//le2 = false
//le2 := cc2 ? true : (le2[1] and low < hp2 ? false : le2[1])

//i = input(0.0)

//length1 = input(1, minval=1, title="Upper Channel")
//length2 = input(1, minval=1, title="Lower Channel")

//upper = highest(length1)
//lower = lowest(length2)

//break_Above = close > upper[1] ? 1 : 0
//break_Below = close < lower[1] ? 1 : 0

//if (le and window() and close<(hp-i))
//    strategy.entry("Long", strategy.long, stop = (hp-i))
//if (le2 and window() and close>(hp2+i))
//    strategy.entry("Short", strategy.short, stop = (hp2+i))

//if  break_Above
//    strategy.close("Long")

//if  break_Below
//    strategy.close("Short")
