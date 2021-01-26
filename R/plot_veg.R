library(raster)
library(rgdal)
library(RColorBrewer)
library(rcarbon)
library(ggplot2)

sam <- readOGR("~/reveals/layers/southamerica.shp")
sam <- spTransform(sam, CRS("+proj=aea +lat_0=-32 +lon_0=-60 +lat_1=-5 
                             +lat_2=-42 +x_0=0 +y_0=0 +ellps=aust_SA +units=m 
                             +no_defs"))

plot_veg <- function(folder) {
    res <- vector(mode="list", length=55)
    names(res) <- as.character(seq(6000, 600, -100))
    for(i in seq(6000, 1000, -500)) {
        r <- raster(paste(folder, "/veg", i, ".asc", sep=""))
        res[[as.character(i)]] <- r
    }
    par(mfrow=c(3, 4), mar=c(0,0,1,0))
    for(i in seq(6000, 1000, -500)) {
        if(i > 1000) {
            plot(res[[as.character(i)]], col=brewer.pal(n=9, name="YlGn"),
                 breaks=seq(0.1,1,0.1), axes=F, box=F, main=paste(i,"cal BP"),
                 legend=F)
        } else {
            plot(res[[as.character(i)]], col=brewer.pal(n=9, name="YlGn"),
                 breaks=seq(0.1,1,0.1), legend.width=4,
                 legend.shrink=0.75, legend.args=list(text="%AP"), axes=F,
                 box=F, main=paste(i,"cal BP"))
        }
        plot(sam, add=T)
    }
}

get_region <- function(df) {
    regions <- as.character(df$name)
    for(i in 1:nrow(df)) {
        if(grepl("queimada_nova", df$name[i]) || grepl("sauipe", df$name[i])) {
            regions[i] <- "NE"
        } else if (grepl("alto_parana", df$name[i])) {
            regions[i] <- "PAR"
        } else if (grepl("morro_grande", df$name[i])) {
            regions[i] <- "ATL"
        } else if (grepl("pedra_talhada", df$name[i])) {
            regions[i] <- "CO"
        } else if (grepl("caldeirao", df$name[i]) || grepl("bela_vista", df$name[i])) {
            regions[i] <- "EAM"
        }
    }
    df$Region <- regions
    return(df)
}

long <- get_region(read.csv("tupi_final_long.csv"))
long$type <- "long"
short <- get_region(read.csv("tupi_final_short.csv"))
short$type <- "short"

encontro_v <- get_region(read.csv("encontro_v/dates.csv"))
encontro_v$model <- "veg"
encontro_v$origin <- "encontro"

encontro <- get_region(read.csv("encontro/dates.csv"))
encontro$model <- "no_veg"
encontro$origin <- "encontro"

urupa_v <- get_region(read.csv("urupa_v/dates.csv"))
urupa_v$model <- "veg"
urupa_v$origin <- "urupa"

urupa <- get_region(read.csv("urupa/dates.csv"))
urupa$model <- "no_veg"
urupa$origin <- "urupa"

reg <- c("NE", "PAR", "ATL", "CO", "EAM")
light_grey = rgb(0.7,0.7,0.7,alpha=0.3)

plot_dates <- function(real, sim) {
    plot(real$x, real$y, type="n", xlim=c(1000,3500))
    b <- real[real$type=="long",]
    polygon(b$x, b$y, col=light_grey, border="grey")
    a <- real[real$type=="short",]
    polygon(a$x, a$y, col="black")
    for(i in 1:nrow(sim)) {
        if(sim$year[i] != -1) {
            col <- list(veg="green", no_veg="yellow")
            shape <- list(encontro=22, urupa=23)
            points(sim$year[i], 0.002, pch=shape[[sim$origin[i]]], bg=col[[sim$model[i]]], cex=1.5)
        }
    }
}

prepare <- function(region) {
    atl_c14 <- rbind(short[short$Region == region,], long[long$Region == region,])

    for(i in 1:nrow(atl_c14)) {
        atl_cal <- calibrate(atl_c14$C14Age[i], atl_c14$C14SD[i], calCurves="shcal20")
        if(i == 1) {
            ATL <- data.frame(x=atl_cal$grid[[1]]$calBP,
                            y=atl_cal$grid[[1]]$PrDens,
                            type=as.character(atl_c14$type[i]))
        } else {
            df <- data.frame(x=atl_cal$grid[[1]]$calBP,
                            y=atl_cal$grid[[1]]$PrDens,
                            type=as.character(atl_c14$type[i]))
            ATL <- rbind(ATL, df)
        }
    }

    b <- rbind(encontro_v[encontro_v$Region == region,],
            encontro[encontro$Region == region,],
            urupa_v[urupa_v$Region == region,],
            urupa[urupa$Region == region,])
            
    plot_dates(ATL, b)
}

par(mfrow=c(5, 1), mar=c(1,1,1,1))

for(i in reg) {prepare(i)}