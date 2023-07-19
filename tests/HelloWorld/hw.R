library(RSQLite)

con <- dbConnect(SQLite(), dbname = "HelloWorld.db")

dbGetQuery(con, "DELETE FROM AOI")

aoi <- data.frame(AOIID = NA_integer_,
                  Link = "itemcell",
                  ID = rep(1:4, each = 2),
                  Name = rep(c("L", "R"), times = 4),
                  Resource = rep(c("sg.png", "dk.png", "dk.png", "sg.png"), times = 2),
                  x1 = rep(c(100, 600), times = 4),
                  y1 = rep(300, times = 8),
                  Layer = 1L,
                  stringsAsFactors = FALSE)

dbWriteTable(con, "AOI", aoi, append = TRUE)

dbDisconnect(con)
