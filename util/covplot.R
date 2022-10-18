coverage <- read.delim("coverage.csv", header=FALSE, row.names=1)


# ~~~~~~~~~~~~~~~~~~~~~~~ VARIABLES ~~~~~~~~~~~~~~~~~~~~~~~
raw_coverage <- coverage[1,2:ncol(coverage)] # ignore 1st position here to have an overlap between plot 1 & 2
fold_change  <- coverage[2,2:ncol(coverage)]
threshold    <- coverage[3,1]


# ~~~~~~~~~~~~~~~~~~~~~~~ PLOTS ~~~~~~~~~~~~~~~~~~~~~~~
par(mfrow=c(3,1), mar = c(4, 4, 1, 1))

# PLOT 1
plot(x=seq_len(length(raw_coverage)),
     y=raw_coverage,
     xlab = "",
     ylab="Absolute genome coverage",
     ylim = c(0, max(raw_coverage)),
     xaxt = "n",
     bty="n"
)

# PLOT 2
plot(x=seq_len(length(fold_change)),
     y=fold_change,
     xlab="window index [bp]",
     ylab="Log2 fold-change",
     bty="n"
)
# select points to color differently
extremeVals <- (fold_change > threshold) | (fold_change < -threshold)
points(seq_len(length(fold_change))[extremeVals], fold_change[extremeVals], pch=19, col="red")
legend("topright", border = "n", pch = 19, col = "red", legend = "|data| > threshold")


# PLOT 3
hist(as.numeric(fold_change), breaks = 50, main = "", xlab = "Log2 fold-change")
abline(v = threshold, col = "red")
abline(v =-threshold, col = "red")
legend("topright", border = "n", lwd = 1, col = "red", legend = "threshold")
