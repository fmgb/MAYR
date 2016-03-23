package com.robotxy.fmgb.xandroidy;

import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;

import android.widget.Button;
import android.widget.CheckBox;
import android.widget.Checkable;
import android.widget.ProgressBar;
import android.widget.RadioButton;
import android.widget.SeekBar;
import android.widget.TextView;

import com.google.android.gms.common.server.converter.StringToIntConverter;

import org.w3c.dom.Text;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;

public class MainActivity extends AppCompatActivity {

    /**
     * The {@link android.support.v4.view.PagerAdapter} that will provide
     * fragments for each of the sections. We use a
     * {@link FragmentPagerAdapter} derivative, which will keep every
     * loaded fragment in memory. If this becomes too memory intensive, it
     * may be best to switch to a
     * {@link android.support.v4.app.FragmentStatePagerAdapter}.
     */
    private SectionsPagerAdapter mSectionsPagerAdapter;

    private ProgressBar pbVertical = null;
    private ProgressBar pbHorizontal = null;
    /**
     * The {@link ViewPager} that will host the section contents.
     */
    private ViewPager mViewPager;
    public static String IP = "0";
    public static String Puerto = "0";
    public static Network net;

    public int posXActual = 0;
    public int posYActual = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        // Set up the ViewPager with the sections adapter.
        mViewPager = (ViewPager) findViewById(R.id.container);

        // Create the adapter that will return a fragment for each of the three
        // primary sections of the activity.
        mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());

        mSectionsPagerAdapter.addFragment(PlaceholderFragment.newInstance(R.layout.fragment_control));
        mSectionsPagerAdapter.addFragment(PlaceholderFragment.newInstance(R.layout.fragment_main));
        mSectionsPagerAdapter.addFragment(PlaceholderFragment.newInstance(R.layout.fragment_status));
        mViewPager.setAdapter(mSectionsPagerAdapter);

       // pbHorizontal = (ProgressBar) findViewById(R.id//.progressBarHorizontal);
        //pbVertical = (ProgressBar) findViewById(R.id.progressBarVertical);

       /* FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });*/
        //Network net = new Network();

    }



    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void conectarInternet()
    {
        net = new Network(IP, Puerto);
    }

    public void onClickConectar(View view) {
        TextView tIP = (TextView) findViewById(R.id.editTextIP);
        TextView tPort = (TextView) findViewById(R.id.editTextPuerto);
        IP = tIP.getText().toString();
        Puerto = tPort.getText().toString();
        conectarInternet();
        String result;
        TextView tvBaud = (TextView) findViewById(R.id.editTextBaudios);
        String baud = tvBaud.getText().toString();
        net.execute(new URLS().CONNECT + baud);
        result = net.conectado;
        System.out.println(result);
        //Intentar añadir un temporizador
        TextView tView = (TextView) findViewById(R.id.textViewConectado);
        tView.setText("Conectado");
        Button button = (Button)  findViewById(R.id.buttonConnect);
        button.setText("Desconectar");
    }

    public void onClickMoveMm(View view){
        conectarInternet();

        TextView tvNumber = (TextView) findViewById(R.id.numberTextId);
        String value = tvNumber.getText().toString();
        RadioButton rbX = (RadioButton) findViewById(R.id.radioX);
        RadioButton rbY = (RadioButton) findViewById(R.id.radioY);
       // boolean isPositive = tvNumber.getText().toString().indexOf("-") ? true : false;
        //Seleccionado eje X
        if(rbX.isChecked())
        {
            //Si es positivo
            if(value.indexOf("-") == -1)
                net.execute(new URLS().MOVE_X_MM_POS + value);
            else
                net.execute(new URLS().MOVE_X_MM_NEG + value.substring(1));
        }
        else if(rbY.isChecked())
        {
            if(value.indexOf("-") == -1)
                net.execute(new URLS().MOVE_Y_MM_POS + value);
            else
                net.execute(new URLS().MOVE_Y_MM_NEG + value.substring(1));
        }
    }


    public void onClickMoveSteps(View view){
        conectarInternet();

        TextView tvNumber = (TextView) findViewById(R.id.numberTextId);
        String value = tvNumber.getText().toString();
        RadioButton rbX = (RadioButton) findViewById(R.id.radioX);
        RadioButton rbY = (RadioButton) findViewById(R.id.radioY);
        // boolean isPositive = tvNumber.getText().toString().indexOf("-") ? true : false;
        //Seleccionado eje X
        if(rbX.isChecked())
        {
            //Si es positivo
            if(value.indexOf("-") == -1)
                net.execute(new URLS().MOVE_X_STEPS_POS + value);
            else
                net.execute(new URLS().MOVE_X_STEPS_NEG + value.substring(1));
        }
        else if(rbY.isChecked())
        {
            if(value.indexOf("-") == -1)
                net.execute(new URLS().MOVE_Y_STEPS_POS + value);
            else
                net.execute(new URLS().MOVE_Y_STEPS_NEG + value.substring(1));
        }
    }

    public void onClickHome(View view)
    {
        RadioButton rbX = (RadioButton) findViewById(R.id.radioX);
        RadioButton rbY = (RadioButton) findViewById(R.id.radioY);
        conectarInternet();
        if(rbX.isChecked())
            net.execute(new URLS().HOME + 0);
        else
            net.execute(new URLS().HOME + 1);
    }


    public void onClickCalibrate(View view)
    {
        RadioButton rbX = (RadioButton) findViewById(R.id.radioX);
        RadioButton rbY = (RadioButton) findViewById(R.id.radioY);
        conectarInternet();
        if(rbX.isChecked())
            net.execute(new URLS().CALIBRATE + 0);
        else
            net.execute(new URLS().CALIBRATE + 1);
    }

    public void onClickRearme(View view)
    {
        conectarInternet();
        net.execute(new URLS().REARME);
    }

    public void onClickUpdate(View view)
    {
        conectarInternet();
        String update = "";
        //String result = net.doInBackground(new URLS().GET_ALARMS);
        try {
            update = net.execute(new URLS().UPDATE).get();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
        //Adapt the result to GET to format endstops
        update = update.substring(9,update.length()-1);
        RadioButton rbMinX = (RadioButton) findViewById(R.id.radioMinX);
        RadioButton rbMinY = (RadioButton) findViewById(R.id.radioMinY);
        RadioButton rbMaxX = (RadioButton) findViewById(R.id.radioMaxX);
        RadioButton rbMaxY = (RadioButton) findViewById(R.id.radioMaxY);
        int endstops = Integer.parseInt(update);
        if(endstops/1000 == 1)
            rbMinX.setChecked(true);
        else
            rbMinX.setChecked(false);
            endstops %= 1000;

        if(endstops/100 == 1)
            rbMaxX.setChecked(true);
        else
            rbMaxX.setChecked(false);
        endstops %= 100;

        if(endstops/10 == 1)
            rbMaxY.setChecked(true);
        else
            rbMaxY.setChecked(false);
        endstops %= 10;

        if(endstops == 1)
            rbMinY.setChecked(true);
        else
            rbMinY.setChecked(false);

        TextView tvStepsX = (TextView) findViewById(R.id.textStepsX);
        TextView tvStepsY = (TextView) findViewById(R.id.textStepsY);
        tvStepsX.setVisibility(View.VISIBLE);
        tvStepsY.setVisibility(View.VISIBLE);
    }

    public void onClickEmergency(View view)
    {
        conectarInternet();
        net.execute(new URLS().EMERGENCIA);
    }

    public void onClickJoystick(View view)
    {
        conectarInternet();
        CheckBox joystick = (CheckBox) findViewById(R.id.checkBoxJoystick);
        if(joystick.isChecked())
            net.execute(new URLS().JOYSTICK + 1);
        else
            net.execute(new URLS().JOYSTICK + 0);
            System.out.println(new URLS().JOYSTICK + 0);
    }

    public void onClickGetPosition(View view)
    {
        // TODO Realizar con .get() en el execute para mostrar la posicion actual.
        conectarInternet();
        String getURL = null;
        try {
            getURL = net.execute(new URLS().GET_POSITION).get();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
        if(getURL != null) {
            String[] positions = getURL.split(",");
            positions[1] = positions[1].substring(0,positions[1].length()-1);
            posXActual = Integer.parseInt(positions[0]);
            posYActual = Integer.parseInt(positions[1]);
            System.out.println(positions[0] + "   " + positions[1]);
            SeekBar sbX = (SeekBar) findViewById(R.id.seekBarX);
            SeekBar sbY = (SeekBar) findViewById(R.id.seekBarY);
            sbX.setProgress(posXActual);
            sbY.setProgress(posYActual);
            // TODO Para comprobar.
  /*          TextView tvXActual = (TextView) findViewById(R.id.tvPosActualX);
            TextView tvYActual = (TextView) findViewById(R.id.tvPosActualY);
            tvXActual.setText(posXActual);
            tvYActual.setText(posYActual);
   */     }
    }



    public void onClickSetPositionX(View view) {
        // TODO Realizar con .get() en el execute para mostrar la posicion actual.
        conectarInternet();
        String getURL = null;
        /*try {
            getURL = net.execute(new URLS().GET_POSITION).get();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }*/
        /*if (getURL != null) {
            String[] positions = getURL.split(",");
            positions[1] = positions[1].substring(0, positions[1].length() - 1);
            posXActual = Integer.parseInt(positions[0]);
            posYActual = Integer.parseInt(positions[1]);*/
            SeekBar sbX = (SeekBar) findViewById(R.id.seekBarX);
            SeekBar sbY = (SeekBar) findViewById(R.id.seekBarY);
            int posXTarget = sbX.getProgress();
            int posYTarget = sbY.getProgress();
            int diffX = posXTarget - posXActual;
            int diffY = posYTarget - posYActual;
        System.out.println(posYTarget);
        System.out.println(diffY);
            //IF PARA COMPROBAR SI LA POSICION TARGET ES MAYOR O MENOR, y enviar los datos correspondientes.
            if (diffX > 0) {
                net.execute(new URLS().MOVE_X_STEPS_POS + diffX);
            } else if (diffX < 0) {
                // Ejecutar mover mm X Pos
                diffX = diffX * -1;
                net.execute(new URLS().MOVE_X_STEPS_NEG + diffX);
            }
            /*if (diffY > 0) {
                // Ejecutar mover mm Y Neg
                net.execute(new URLS().MOVE_Y_STEPS_POS + diffY);
            } else if (diffY < 0) {
                diffY = diffY * -1;
                //Ejecutar mover mm Y POs
                net.execute(new URLS().MOVE_Y_STEPS_NEG + diffY);
            }*/
            // TODO Para comprobar.
           /* TextView tvXActual = (TextView) findViewById(R.id.tvPosActualX);
            TextView tvYActual = (TextView) findViewById(R.id.tvPosActualY);
            tvXActual.setText(posXTarget);
            tvYActual.setText(posYTarget);
        */
    }

    public void onClickSetPositionY(View view) {
        // TODO Realizar con .get() en el execute para mostrar la posicion actual.
        conectarInternet();
        String getURL = null;
        /*try {
            getURL = net.execute(new URLS().GET_POSITION).get();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }*/
        /*if (getURL != null) {
            String[] positions = getURL.split(",");
            positions[1] = positions[1].substring(0, positions[1].length() - 1);
            posXActual = Integer.parseInt(positions[0]);
            posYActual = Integer.parseInt(positions[1]);*/
        SeekBar sbX = (SeekBar) findViewById(R.id.seekBarX);
        SeekBar sbY = (SeekBar) findViewById(R.id.seekBarY);
        int posXTarget = sbX.getProgress();
        int posYTarget = sbY.getProgress();
        int diffX = posXTarget - posXActual;
        int diffY = posYTarget - posYActual;
        System.out.println(posYTarget);
        System.out.println(diffY);
        //IF PARA COMPROBAR SI LA POSICION TARGET ES MAYOR O MENOR, y enviar los datos correspondientes.
        /*if (diffX > 0) {
            net.execute(new URLS().MOVE_X_STEPS_POS + diffX);
        } else if (diffX < 0) {
            // Ejecutar mover mm X Pos
            diffX = diffX * -1;
            net.execute(new URLS().MOVE_X_STEPS_NEG + diffX);
        }*/
        if (diffY > 0) {
            // Ejecutar mover mm Y Neg
            net.execute(new URLS().MOVE_Y_STEPS_POS + diffY);
        } else if (diffY < 0) {
            diffY = diffY * -1;
            //Ejecutar mover mm Y POs
            net.execute(new URLS().MOVE_Y_STEPS_NEG + diffY);
        }
        // TODO Para comprobar.
           /* TextView tvXActual = (TextView) findViewById(R.id.tvPosActualX);
            TextView tvYActual = (TextView) findViewById(R.id.tvPosActualY);
            tvXActual.setText(posXTarget);
            tvYActual.setText(posYTarget);
        */
    }
    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {
        static int i;
        /**
         * The fragment argument representing the section number for this
         * fragment.
         */
        private static final String ARG_SECTION_NUMBER = "section_number";

        public PlaceholderFragment() {
            i = 0;
        }

        /**
         * Returns a new instance of this fragment for the given section
         * number.
         */
        public static PlaceholderFragment newInstance(int sectionNumber) {
            PlaceholderFragment fragment = new PlaceholderFragment();
            Bundle args = new Bundle();

            args.putInt(ARG_SECTION_NUMBER, sectionNumber);
            fragment.setArguments(args);
            return fragment;
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {

            View rootView;
            if (i == 0) {
                rootView = inflater.inflate(R.layout.fragment_control, container, false);
                //i = 1;
            } else if (i == 1) {
                rootView = inflater.inflate(R.layout.fragment_main, container, false);
                //i = 2;
            } else if (i == 2)
            {
                rootView = inflater.inflate(R.layout.fragment_status, container, false);
                //i = 0;
            }else
                rootView = inflater.inflate(R.layout.fragment_status, container, false);
            //Para poner mas fragments ir anyadiendo else if
            i = (i +1)%3;
            System.out.println(i);
            return rootView;
        }

    }

    /**
     * A {@link FragmentPagerAdapter} that returns a fragment corresponding to
     * one of the sections/tabs/pages.
     */
    public class SectionsPagerAdapter extends FragmentPagerAdapter {

        List<Fragment> fragments;

        public SectionsPagerAdapter(FragmentManager fm) {
            super(fm);
            this.fragments = new ArrayList<Fragment>();
        }

        public void addFragment(Fragment fragment)
        {
            this.fragments.add(fragment);
        }

        @Override
        public Fragment getItem(int position) {

            // getItem is called to instantiate the fragment for the given page.
            // Return a PlaceholderFragment (defined as a static inner class below).
            return this.fragments.get(position);
        }

        @Override
        public int getCount() {
            return this.fragments.size();
        }

        @Override
        public CharSequence getPageTitle(int position) {

            switch (position) {
                case 0:
                    return "CONECTAR";
                case 1:
                    return "CONTROL";
                case 2:
                    return "STATUS";
            }
            return null;
        }
    }
}
