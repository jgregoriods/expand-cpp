library(raster)
library(RColorBrewer)

plot_veg <- function() {
    res <- vector(mode="list", length=55)
    names(res) <- as.character(seq(6000, 600, -100))
    for(i in seq(6000, 600, -100)) {
        r <- raster(paste("layers/veg/veg", i, ".asc", sep=""))
        r[values(r) < 0] <- 0
        r[values(r) > 1] <- 1
        res[[as.character(i)]] <- r
    }

}