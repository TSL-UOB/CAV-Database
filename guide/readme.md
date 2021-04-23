# Making a table in SQL

Let's generate a static lookup table that can be used to access some data. 

Create a schema called "sim_log":

``` sql
CREATE SCHEMA sim_log;
```

Create a table called "dcd_lookup" under the "sim_log" schema with the correct headings and set the variable, include an ID

``` sql
create table sim_log.dcd_lookup
(dcd_id serial,
vEV_mph float,
vVA_mph float,
vOV_mph float,
DCD_m float,
vEV_mps float,
vVA_mps float,
vOV_mps float);
```

