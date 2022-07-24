CREATE TABLE IF NOT EXISTS artists
(
    id INTEGER PRIMARY KEY NOT NULL,
    name TEXT NOT NULL DEFAULT '',
    sortableName TEXT,
    website TEXT,

    isLiked INTEGER NOT NULL DEFAULT FALSE,
    playCount INTEGER NOT NULL DEFAULT 0,
    skipCount INTEGER NOT NULL DEFAULT 0,
    lastTimePlayed INTEGER NOT NULL DEFAULT 0 /* epoch */
);

CREATE TABLE IF NOT EXISTS albums
(
    id INTEGER PRIMARY KEY NOT NULL,
    title TEXT NOT NULL DEFAULT '',
    sortableTitle TEXT,
    artistID INTEGER NOT NULL DEFAULT 0,
    consideredDate TEXT,
    originalDate TEXT,
    year INTEGER,
    originalYear INTEGER,
    totalDiscs INTEGER NOT NULL DEFAULT 1,
    totalTracks INTEGER,
    ASIN TEXT,
    barcode TEXT,
    catalogNumber TEXT,
    country TEXT,
    ISRC TEXT,
    media TEXT,
    mixDJ TEXT,
    recordLabel TEXT,
    replayGain REAL,
    replayGainPeak REAL,
    replayGainRange REAL,
    replayGainReferenceLoudness REAL,
    script TEXT,
    status TEXT,
    type TEXT,

    originalDecade INTEGER,
    isLiked INTEGER NOT NULL DEFAULT FALSE,
    playCount INTEGER NOT NULL DEFAULT 0,
    skipCount INTEGER NOT NULL DEFAULT 0,
    lastTimePlayed INTEGER NOT NULL DEFAULT 0, /* epoch */

    FOREIGN KEY (artistID) REFERENCES artists (id)
        ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS tracks
(
    id INTEGER PRIMARY KEY NOT NULL,
    knownFilepath TEXT NOT NULL UNIQUE,
    title TEXT NOT NULL DEFAULT '',
    sortableTitle TEXT,
    mainArtistID INTEGER NOT NULL DEFAULT 0,
    albumID INTEGER NOT NULL DEFAULT 0,
    trackNumber INTEGER NOT NULL DEFAULT 0,
    discNumber INTEGER DEFAULT 1,
    duration INTEGER NOT NULL DEFAULT 0,
    AcoustID TEXT,
    AcoustIDFingerprint TEXT,
    arranger TEXT,
    BPM INTEGER,
    comm TEXT, /* comment tag */
    isCompilation INTEGER DEFAULT FALSE,
    composer TEXT,
    sortableComposer TEXT,
    conductor TEXT,
    discSubtitle TEXT,
    encodedBy TEXT,
    encoderSettings TEXT,
    engineer TEXT,
    gaplessPlayback INTEGER DEFAULT FALSE,
    genre TEXT,
    key TEXT,
    language TEXT,
    license TEXT,
    lyricist TEXT,
    lyrics TEXT,
    mixer TEXT,
    mood TEXT,
    originalAlbumID INTEGER,
    originalArtistID INTEGER,
    producer TEXT,
    rating INTEGER,
    remixer TEXT,
    replayGain REAL,
    replayGainPeak REAL,
    replayGainRange REAL,
    subtitle TEXT,
    workTitle TEXT,
    writer TEXT,
    exoticMetadata TEXT,

    isLiked INTEGER NOT NULL DEFAULT FALSE,
    playCount INTEGER NOT NULL DEFAULT 0,
    skipCount INTEGER NOT NULL DEFAULT 0,
    lastTimePlayed INTEGER NOT NULL DEFAULT 0, /* epoch */

    FOREIGN KEY (mainArtistID) REFERENCES artists (id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (albumID) REFERENCES albums (id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (originalAlbumID) REFERENCES albums (id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (originalArtistID) REFERENCES artists (id)
        ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS artists_tracks
(
    trackID INTEGER NOT NULL,
    artistID INTEGER NOT NULL,

    FOREIGN KEY (trackID) REFERENCES tracks (id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (artistID) REFERENCES artists (id)
        ON DELETE CASCADE ON UPDATE CASCADE
);

INSERT OR IGNORE INTO artists (id, name) VALUES (0, '[?]');
INSERT OR IGNORE INTO albums (id, title, artistID) VALUES (0, '[?]', 0);
