import React from "react";
import Header from "./Header";
import { Button, Grid } from '@mui/material';
import WorkoutSelectionCard from "./WorkoutSelectionCard";
import Typography from '@mui/material/Typography';

export default function StartScreen() {
    return(
        <div>
            <Header></Header>
            <Typography gutterBottom variant="h4" component="div">
                Welcome to Plyo-Metrics
            </Typography>
            <Typography gutterBottom variant="h5" component="div">
                Please Select a Workout:
            </Typography>
            <Grid 
                container
                direction="row"
                alignItems="center"
                justifyContent="center"
                style={{padding: '1%'}}
            >
                <Grid item xs={5} style={{padding: '1%'}}>
                    <WorkoutSelectionCard imgSrc="walPush.JPG" title="Wall Push Up"></WorkoutSelectionCard>
                </Grid>
                <Grid item xs={5} style={{padding: '1%'}}>
                    <WorkoutSelectionCard imgSrc="legPress.JPG" title="Leg Press"></WorkoutSelectionCard>
                </Grid>
                <Grid item xs={5} style={{padding: '1%'}}>
                    <WorkoutSelectionCard imgSrc="pushUp.JPG" title="Standard Push Up"></WorkoutSelectionCard>
                </Grid>
                <Grid item xs={5} style={{padding: '1%'}}>
                    <WorkoutSelectionCard imgSrc="calfPlyo.JPG" title="Calf Raises"></WorkoutSelectionCard>
                </Grid>
            </Grid>
        </div>
    );
};
