import React from "react";
import Header from "./Header";
import { Grid } from '@mui/material';
import WorkoutSelectionCard from "./WorkoutSelectionCard";
import Typography from '@mui/material/Typography';

export default function StartScreen() {
    return(
        <div style={{backgroundColor: '#121212', height: '100%'}}>
            <Header></Header>
            <Typography gutterBottom variant="h4" component="div" style={{textAlign: 'center', color: '#ffffff'}}>
                Welcome to ISS Plyometric Workout Station
            </Typography>
            <Typography gutterBottom variant="h5" component="div" style={{textAlign: 'center', color: '#ffffff'}}>
                Please Select a Workout:
            </Typography>
            <Grid 
                container
                direction="row"
                alignItems="center"
                justifyContent="center"
                style={{padding: '1%', textAlign: 'center'}}
            >
                <Grid item xs={5} style={{padding: '1%'}} align="center">
                    <WorkoutSelectionCard imgSrc="walPush.JPG" title="Wall Push Up"></WorkoutSelectionCard>
                </Grid>
                <Grid item xs={5} style={{padding: '1%'}} align="center">
                    <WorkoutSelectionCard imgSrc="legPress.JPG" title="Leg Press"></WorkoutSelectionCard>
                </Grid>
                <Grid item xs={5} style={{padding: '1%'}} align="center">
                    <WorkoutSelectionCard imgSrc="pushUp.JPG" title="Standard Push Up"></WorkoutSelectionCard>
                </Grid>
                <Grid item xs={5} style={{padding: '1%'}} align="center">
                    <WorkoutSelectionCard imgSrc="calfPlyo.JPG" title="Calf Raises"></WorkoutSelectionCard>
                </Grid>
            </Grid>
        </div>
    );
};
