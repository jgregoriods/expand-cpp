library(rgdal)
library(sp)
library(ggplot2)
library(raster)
library(viridis)

plotDates <- function() {

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
