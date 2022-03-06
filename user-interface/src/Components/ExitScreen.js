import React from "react";
import Header from "./Header";
import WorkoutChart from "./WorkoutChart";
import { Button, Grid } from '@mui/material';
import WorkoutSelectionCard from "./WorkoutSelectionCard";
import SummaryTable from "./SummaryTable";

export default function ExitScreen() {
    return(
        <div>
            <Header></Header>
            <h1>Workout Summary</h1>
            
            <Grid 
                container
                direction="row"
                alignItems="center"
                justifyContent="center"
                style={{padding: '1%'}}
            >
                <Grid item xs={8} style={{padding: '1%'}}>
                <WorkoutChart></WorkoutChart>
                </Grid>
                <Grid item xs={4} style={{padding: '1%'}}>
                    <SummaryTable></SummaryTable>
                </Grid>
            </Grid>
            
        </div>
    );
};
