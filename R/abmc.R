library(rgdal)
library(sp)
library(ggplot2)
library(raster)
library(viridis)
library(rcarbon)

get_regions <- function(names) {
    regions <- as.character(names)
    for (i in 1:length(names)) {
        if (names[i] == "dates/tupi/queimada_nova.txt") {
            regions[i] <- "NE"
        } else if (names[i] == "dates/tupi/alto_parana.txt") {
            regions[i] <- "PAR"
        } else if (names[i] == "dates/tupi/morro_grande_b.txt") {
            regions[i] <- "ATL"
        } else if (names[i] == "dates/tupi/pedra_talhada_b.txt") {
            regions[i] <- "CO"
        } else if (names[i] == "dates/tupi/caldeirao.txt") {
            regions[i] <- "EAM"
        }
    }
    return (regions)
}

plotDates <- function() {
    long <- read.csv("tupi_final_long.csv")
    short <- read.csv("tupi_final_short.csv")

    long_cal <- calibrate(long$C14Age, long$C14SD, calCurves="shcal20")
    res <- matrix(ncol=4, nrow=length(long_cal))
    for (i in 1:length(long_cal)) {
        m <- as.numeric(summary(long_cal[i])$MedianBP)
        min_bp <- as.numeric(strsplit(as.character(summary(long_cal[i])["TwoSigma_BP_1"][1,]), " ")[[1]][1])
        for (j in 1:4) {
            if (paste("TwoSigma_BP_", j, sep="") %in% colnames(summary(long_cal[i]))) {
                last <- paste("TwoSigma_BP_", j, sep="")
            }
        }
        max_bp <- as.numeric(strsplit(as.character(summary(long_cal[i])[last][1,]), " ")[[1]][3])
        res[i, 1] <- as.character(long[i,]$Region)
        res[i, 2] <- as.numeric(m)
        res[i, 3] <- as.numeric(min_bp)
        res[i, 4] <- as.numeric(max_bp)
    }
    res <- as.data.frame(res)
    colnames(res) <- c("Region", "median", "min_bp", "max_bp")
    res$site <- as.character(res$Region)
    res$median <- as.numeric(as.character(res$median))
    res$min_bp <- as.numeric(as.character(res$min_bp))
    res$max_bp <- as.numeric(as.character(res$max_bp))

    short_cal <- calibrate(short$C14Age, short$C14SD, calCurves="shcal20")
    res1 <- matrix(ncol=4, nrow=length(short_cal))
    for (i in 1:length(short_cal)) {
        m <- as.numeric(summary(short_cal[i])$MedianBP)
        min_bp <- as.numeric(strsplit(as.character(summary(short_cal[i])["TwoSigma_BP_1"][1,]), " ")[[1]][1])
        for (j in 1:4) {
            if (paste("TwoSigma_BP_", j, sep="") %in% colnames(summary(short_cal[i]))) {
                last <- paste("TwoSigma_BP_", j, sep="")
            }
        }
        max_bp <- as.numeric(strsplit(as.character(summary(short_cal[i])[last][1,]), " ")[[1]][3])
        res1[i, 1] <- as.character(short[i,]$Region)
        res1[i, 2] <- as.numeric(m)
        res1[i, 3] <- as.numeric(min_bp)
        res1[i, 4] <- as.numeric(max_bp)
    }
    res1 <- as.data.frame(res1)
    colnames(res1) <- c("Region", "median", "min_bp", "max_bp")
    res1$site <- as.character(res1$Region)
    res1$median <- as.numeric(as.character(res1$median))
    res1$min_bp <- as.numeric(as.character(res1$min_bp))
    res1$max_bp <- as.numeric(as.character(res1$max_bp))

    encontro_veg <- read.csv("encontro_v/dates.csv")
    encontro_veg$Region <- get_regions(encontro_veg$name)

    urupa_veg <-read.csv("urupa_v/dates.csv")
    urupa_veg$Region <- get_regions(urupa_veg$name)
    
    encontro_noveg <- read.csv("encontro/dates.csv")
    encontro_noveg$Region <- get_regions(encontro_noveg$name)

    urupa_noveg <- read.csv("urupa/dates.csv")
    urupa_noveg$Region <- get_regions(urupa_noveg$name)

    g <- ggplot() +
            geom_errorbarh(res, mapping=aes(y=Region, xmin=max_bp, xmax=min_bp), colour="grey", height=0.2) +
            geom_point(res, mapping=aes(y=Region, x=median), shape=21, size=2, fill="white", colour="grey") +
            geom_errorbarh(res1, mapping=aes(y=Region, xmin=max_bp, xmax=min_bp), height=0.2) +
            geom_point(res1, mapping=aes(y=Region, x=median), shape=21, size=2, fill="white") +
            geom_point(encontro_noveg, mapping=aes(y=Region, x=year), shape=21, size=2, fill="yellow") +
            geom_point(urupa_noveg, mapping=aes(y=Region, x=year), shape=23, size=2, fill="yellow") +
            geom_point(encontro_veg, mapping=aes(y=Region, x=year), shape=21, size=2, fill="green") +
            geom_point(urupa_veg, mapping=aes(y=Region, x=year), shape=23, size=2, fill="green") +
            xlim(1000, 3500) +
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
                scale_fill_viridis() +
                geom_polygon(data=sam, aes(x = long, y = lat, group = id),
                             colour="black", fill=NA, lwd=0.4) +
                geom_point(data=dates_wrong, aes(x=x, y=y), shape=21, size=3, fill="black", colour="white") +
                geom_point(data=dates_right, aes(x=x, y=y), shape=21, size=3, fill="white", colour="black") +                
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
