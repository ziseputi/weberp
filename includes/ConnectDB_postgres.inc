<?php
/* $Id: ConnectDB_postgres.inc 7751 2017-04-13 16:34:26Z rchacon $ */
/* Database abstraction for postgres */

define ('LIKE','ILIKE');
/* $PgConnStr = $PgConnStr = "host=".$host." dbname=".$_SESSION['DatabaseName']; */
$PgConnStr = 'dbname='.$_SESSION['DatabaseName'];

if( isset($host) && ($host != "")) {
	$PgConnStr = 'host='.$host.' '.$PgConnStr;
}

if( isset( $DBUser ) && ($DBUser != "") ) {
	// if we have a user we need to use password if supplied
	$PgConnStr .= " user=".$DBUser;
	if( isset( $DBPassword ) && ($DBPassword != "") ) {
		$PgConnStr .= " password=".$DBPassword;
	}
}

global $db;		// Make sure it IS global, regardless of our context
$db = pg_connect( $PgConnStr );

if( !$db ) {
	if($debug==1) {
		echo '<BR>' . $PgConnStr . '<BR>';
	}
	echo '<BR>' . _('The company name entered together with the configuration in the file config.php for the database user name and password do not provide the information required to connect to the database.') . '<BR><BR>' . _(' Try logging in with an alternative company name.');
	echo '<BR><A HREF="index.php">' . _('Back to login page') . '</A>';
	unset($_SESSION['DatabaseName']);
	exit;
}

require_once ($PathPrefix .'includes/MiscFunctions.php');

//DB wrapper functions to change only once for whole application

function DB_query ($SQL,
				$ErrorMessage='',
				$DebugMessage= '',
				$Transaction=false,
				$TrapErrors=true) {

	global $debug;
	global $PathPrefix;

	$result = pg_query($db, $SQL);
	if($DebugMessage == '') {
		$DebugMessage = _('The SQL that failed was:');
	}
	//if(DB_error_no($Conn) != 0) {
	if( !$result AND $TrapErrors) {
		if($TrapErrors) {
			require_once($PathPrefix . 'includes/header.php');
		}
		prnMsg($ErrorMessage . '<BR>' . DB_error_msg(),'error', _('DB ERROR:'));
		if($debug==1) {
			echo '<BR>' . $DebugMessage. "<BR>$SQL<BR>";
		}
		if($Transaction) {
			$SQL = 'rollback';
			$Result = DB_query($SQL);
			if(DB_error_no($Conn) !=0) {
				prnMsg('<br />' . _('Error Rolling Back Transaction!!'), '', _('DB DEBUG:') );
			}
		}
		if($TrapErrors) {
			include($PathPrefix . 'includes/footer.php');
			exit;
		}
	}
	return $result;

}

function DB_fetch_row($ResultIndex) {
	$RowPointer=pg_fetch_row($ResultIndex);
	return $RowPointer;
}

function DB_fetch_assoc($ResultIndex) {
	$RowPointer=pg_fetch_assoc($ResultIndex);
	return $RowPointer;
}

function DB_fetch_array($ResultIndex) {
	$RowPointer = pg_fetch_array($ResultIndex);
	return $RowPointer;
}

function DB_data_seek(&$ResultIndex,$Record) {
	pg_result_seek($ResultIndex,$Record);
}

function DB_free_result($ResultIndex) {
	pg_free_result($ResultIndex);
}

function DB_num_rows($ResultIndex) {
	return pg_num_rows($ResultIndex);
}
// Added by MGT
function DB_affected_rows($ResultIndex) {
	return pg_affected_rows($ResultIndex);
}

function DB_error_no() {
	return DB_error_msg() == ""?0:-1;
}

function DB_error_msg() {
	return pg_last_error($db);
}

function DB_Last_Insert_ID(&$Conn, $table, $fieldname) {
	$tempres = DB_query ("SELECT currval('".$table."_".$fieldname."_seq') FROM ".$table);
	$Res = pg_fetch_result( $tempres, 0, 0 );
	DB_free_result($tempres);
	return $Res;
}

function DB_escape_string($String) {
	return pg_escape_string(htmlspecialchars($String, ENT_COMPAT, 'ISO-8859-1'));
}

function INTERVAL( $val, $Inter ) {
	global $dbtype;
	return "\n(CAST( (" . $val . ") as text ) || ' ". $Inter ."')::interval\n";
}
function DB_show_tables(&$Conn) {
	$Result =DB_query("SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'");
	return $Result;
}
function DB_show_fields($TableName,&$Conn) {
	$Result = DB_query("SELECT table_name FROM information_schema.tables WHERE table_schema='public' AND table_name='" . $TableName . "'");
	if(DB_num_rows($Result)==1) {
		$Result = DB_query("SELECT column_name FROM information_schema.columns WHERE table_name ='$TableName'");
		return $Result;
	}
}
function DB_Maintenance() {

	prnMsg(_('The system has just run the regular database administration and optimisation routine'),'info');

	$Result = DB_query('VACUUM ANALYZE');

	$Result = DB_query("UPDATE config
				SET confvalue='" . Date('Y-m-d') . "'
				WHERE confname='DB_Maintenance_LastRun'");
}

?>
