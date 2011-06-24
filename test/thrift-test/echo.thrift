struct Info {
    1: i64 id,
    2: string name,
    3: i32 size
}

service Echo {
    i32 GetInt(1: i32 num),
    i32 GetIntAsync(1: i32 num),
    string GetString(1: string str),
    Info GetInfo()
}


