library(rgdal)
library(sp)
library(ggplot2)
library(raster)
library(viridis)
library(rcarbon)

plotDates <- function(dates) {
    a <- calibrate(dates$C14Age, dates$C14SD, calCurves="shcal20")
    res <- matrix(ncol=4, nrow=length(a))
    for (i in 1:length(a)) {
        m <- as.numeric(summary(a[i])$MedianBP)
        min_bp <- as.numeric(strsplit(as.character(summary(a[i])["TwoSigma_BP_1"][1,]), " ")[[1]][1])
        for (j in 1:4) {
            if (paste("TwoSigma_BP_", j, sep="") %in% colnames(summary(a[i]))) {
                last <- paste("TwoSigma_BP_", j, sep="")
            }
        }
        max_bp <- as.numeric(strsplit(as.character(summary(a[i])[last][1,]), " ")[[1]][3])
        res[i, 1] <- as.character(dates[i,]$Site)
        res[i, 2] <- as.numeric(m)
        res[i, 3] <- as.numeric(min_bp)
        res[i, 4] <- as.numeric(max_bp)
    }
    res <- as.data.frame(res)
    colnames(res) <- c("site", "median", "min_bp", "max_bp")
    res$site <- as.character(res$site)
    res$median <- as.numeric(as.character(res$median))
    res$min_bp <- as.numeric(as.character(res$min_bp))
    res$max_bp <- as.numeric(as.character(res$max_bp))
    
    g <- ggplot() +
            geom_errorbarh(res, mapping=aes(y=site, xmin=max_bp, xmax=min_bp), height=0.2) +
            geom_point(res, mapping=aes(y=site, x=median)) +
            theme_classic()
    return(g)
}

plotMap <- function(folder) {
    sam <- readOGR("layers/southamerica.shp")
    dates <- read.csv(paste(folder, "/dates.csv", sep=""))
    coordinates(dates) <- ~x+y
    proj4string(dates) <- CRS("+proj=aea +lat_1=-5 +lat_2=-42 +lat_0=-32 +lon_0=-60
                               +x_0=0 +y_0=0 +ellps=aust_SA +units=m")
    dates <- spTransform(dates, CRS("+init=epsg:4326"))
    dates <- as.data.frame(dates)
    dates_right <- dates[dates$score > 0,]
    dates_wrong <- dates[dates$score == 0,]
    map <- raster(paste(folder, "/arrival.asc", sep=""))
    proj4string(map) <- CRS("+proj=aea +lat_1=-5 +lat_2=-42 +lat_0=-32 +lon_0=-60
                             +x_0=0 +y_0=0 +ellps=aust_SA +units=m")
    r <- raster(extent(sam))
    res(r) <- 0.1
    proj4string(r) <- CRS("+init=epsg:4326")
    map <- projectRaster(map, r)
    map.p <- rasterToPoints(map)
    df <- data.frame(map.p)
    p <- ggplot(data=df, aes(x=x, y=y)) +
                geom_polygon(data=sam, aes(x = long, y = lat, group = id),
                             colour=NA, fill=NA) +
                geom_raster(aes(fill=arrival)) +
                scale_fill_viridis(option="plasma") +
                geom_polygon(data=sam, aes(x = long, y = lat, group = id),
                             colour="black", fill=NA, lwd=0.4) +
                geom_point(data=dates_right, aes(x=x, y=y), shape=21, size=3, fill="white", colour="black") +
                geom_point(data=dates_wrong, aes(x=x, y=y), shape=21, size=3, fill="black", colour="white") +
                theme_bw() +
                coord_equal() +
                theme(panel.grid.major = element_blank(),
                      panel.grid.minor = element_blank(),
                      panel.border = element_blank(),
                      axis.ticks = element_blank(),
                      axis.text = element_blank(),
                      axis.title.x = element_blank(),
                      axis.title.y = element_blank())
    return(p)
}
